import {WebSocket, WebSocketServer} from 'ws';
import {getConfig, readConfig} from './config';
import {clearChatUsernames, getRandomTwitchNickname, getTwitchUser, startListeningChat} from './twitch';
import {
	getPollNames,
	getVotesArray,
	getWinnerIndex,
	IsVotingEnabled,
	resetPoll,
	setVotingActive,
	updateEffectNamesFromGame
} from './voting';

let gameWebSocketClient: WebSocket | null = null;
let clientReconnectInterval: NodeJS.Timer | null = null;


async function onModEnabled(): Promise<void>
{
	try
	{
		await readConfig();
		
		const login = await getTwitchUser();
		
		if (login)
		{
			startListeningChat(login, getWebSocket);
		}
	}
	catch (err)
	{
		console.error(err);
	}
}

let reconnectsCount = 0;
/**
 * False - send 1-4 options
 * True - send 5-8 options
 */
let bPrevOptionsType = false;

export function getWebSocket(): WebSocket | null
{
	return gameWebSocketClient;
}

export function connectWebsocketClient(): void
{
	const _config = getConfig();
	let maxOptions = _config.max_options;
	if (maxOptions != 4 && maxOptions != 8)
	{
		maxOptions = 4;
	}
	
	gameWebSocketClient = new WebSocket('ws://127.0.0.1:9149');
	
	gameWebSocketClient.on('open', () =>
	{
		if (clientReconnectInterval)
		{
			clearInterval(clientReconnectInterval);
			clientReconnectInterval = null;
		}
		
		reconnectsCount = 0;
	});
	
	gameWebSocketClient.on('error', (err) =>
	{
		// console.error(err);
	});
	
	gameWebSocketClient.on('message', async (data) =>
	{
		if (!gameWebSocketClient)
		{
			return;
		}
		
		try
		{
			const msg = data.toString();
			console.log(msg);
			
			function resetPollAndSend()
			{
				resetPoll();
				sendVotes(getVotesArray(), _config.weighted_voting);
				updatePollOptions(getPollNames(), _config.weighted_voting);
			}
			
			switch (msg)
			{
				case 'mod_enabled':
					onModEnabled();
					resetPollAndSend();
					setPollVisible(false);
					clearChatUsernames();
					bPrevOptionsType = false;
					break;
				case 'mod_disabled':
				{
					setVotingActive(false);
					resetPollAndSend();
					setPollVisible(false);
					clearChatUsernames();
					break;
				}
				case 'vote_ended':
				{
					setPollVisible(true);
					sendVotes(getVotesArray(), _config.weighted_voting);
					setVotingActive(false);
					
					const winner: number = getWinnerIndex(_config.weighted_voting);
					
					updateWinner(winner);
					
					gameWebSocketClient.send(JSON.stringify({
						type: "activate-effect",
						index: winner,
					}));
					
					setPollFadeOut();
					break;
				}
				case 'request-twitch-viewer-name':
				{
					gameWebSocketClient.send(JSON.stringify({
						type: "spawn-twitch-viewer",
						name: getRandomTwitchNickname(),
					}));
					break;
				}
				default:
				{
					try
					{
						const parsedJSON = JSON.parse(msg);
						
						switch (parsedJSON['type'])
						{
							case 'vote_activate':
								const effectNames = parsedJSON['data'];
								
								updateEffectNamesFromGame(effectNames);
								setVotingActive(true);
								sendVotes(getVotesArray(), _config.weighted_voting);
								updatePollOptions(getPollNames(), _config.weighted_voting);
								setPollVisible(true);
								if (maxOptions != 4)
								{
									setPollOptionsNumber(bPrevOptionsType);
									
									bPrevOptionsType = !bPrevOptionsType;
								}
								break;
						}
					}
					catch (err)
					{
						console.error(err);
					}
				}
					break;
			}
		}
		catch (error)
		{
			console.error(error);
		}
	});
	
	gameWebSocketClient.on('close', () =>
	{
		reconnectsCount++;
		
		if (reconnectsCount >= 20)
		{
			process.exit(0);
			return;
		}
		
		
		if (clientReconnectInterval)
		{
			clearInterval(clientReconnectInterval);
			clientReconnectInterval = null;
		}
		
		console.log('[GameSocket] Reconnecting..');
		
		clientReconnectInterval = setInterval(() =>
		{
			connectWebsocketClient();
		}, 2500);
	});
}

const overlayClients: Array<WebSocket> = [];

let overlayServer: WebSocketServer | null = null;

function sendVotes(votes: Array<number>, weighted_voting: boolean)
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({type: 'update-votes', data: votes, weighted_voting: weighted_voting}));
		}
	}
}

function updatePollOptions(options: Array<string>, weighted_voting: boolean)
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({type: 'new-options', data: options, weighted_voting: weighted_voting}));
		}
	}
}

function updateWinner(winner: number)
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({type: 'set-winner', data: winner}));
		}
	}
}


function setPollStarted()
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({type: 'poll-started'}));
		}
	}
}

function setPollVisible(bVisible: boolean)
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({type: 'set-poll-visible', data: bVisible}));
		}
	}
}

function setPollFadeOut()
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({type: 'set-poll-fade-out'}));
		}
	}
}

function setPollOptionsNumber(bMoreOptions: boolean)
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({type: 'set-poll-options-number', data: bMoreOptions}));
		}
	}
}

export function startWSServer(bLocalIP: boolean = true)
{
	const ip = bLocalIP ? '127.0.0.1' : '0.0.0.0';
	overlayServer = new WebSocketServer({port: 9147, host: ip});
	
	// setVotingActive(true);
	// setRandomPollOptions();
	
	overlayServer.on('connection', (ws) =>
	{
		
		overlayClients.push(ws);
		let votesInterval: NodeJS.Timer | null = null;
		
		ws.on('message', function message(data)
		{
			console.log('received: %s', data);
		});
		
		ws.on('close', () =>
		{
			if (votesInterval)
			{
				clearInterval(votesInterval);
				votesInterval = null;
			}
			
			const index = overlayClients.indexOf(ws);
			
			if (index != -1)
			{
				overlayClients.splice(index, 1);
			}
		});
		
		function sendVotes(votes: Array<number>): void
		{
			ws.send(JSON.stringify({type: 'update-votes', data: votes}));
		}
		
		votesInterval = setInterval(() =>
		{
			if (IsVotingEnabled())
			{
				sendVotes(getVotesArray());
			}
		}, 1000);
		
		ws.send(JSON.stringify({type: 'new-options', data: getPollNames()}));
		
	});
}


