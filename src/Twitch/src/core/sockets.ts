import { WebSocketServer, WebSocket } from 'ws';
import { getConfig, readConfig } from './config';
import { randomInteger } from './index';
import { getTwitchUser, startListeningChat } from './twitch';
import { disabledEffects, effectsList, getPollNames, getVotesArray, getWInnerData, getWinnerEffect, getWinnerIDByVotes, IsVotingEnabled, reactivateDisabledEffects, resetPoll, setRandomPollOptions, setVotingActive, tempDisableEffect } from './voting';

let gameWebSocketClient: WebSocket | null = null;
let clientReconnectInterval: NodeJS.Timer | null = null;


async function onModEnabled(): Promise<void>
{
	try
	{
		await readConfig();

		const conf_ = getConfig();
	
		const login = await getTwitchUser();
	
		if (login)
		{
			startListeningChat(login);
		}
	
		if (gameWebSocketClient && conf_.interval && conf_.votingDuration)
		{
			gameWebSocketClient.send(JSON.stringify({ type: "update-interval", interval: conf_.interval, voting: conf_.votingDuration  }));
		}
	}
	catch (err)
	{
		console.error(err);
	}
}

let reconnectsCount = 0;

export function connectWebsocketClient(): void
{
	gameWebSocketClient = new WebSocket('ws://localhost:9149');

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
				sendVotes(getVotesArray());
				updatePollOptions(getPollNames());
			}

			switch (msg)
			{
				case 'mod_enabled':
					onModEnabled();
					resetPollAndSend();
					setPollVisible(false);
					break;
				case 'mod_disabled':
					{
						setVotingActive(false);
						resetPollAndSend();
						setPollVisible(false);

						break;	
					}
				case 'vote_activate':
					{
						setRandomPollOptions();
						setVotingActive(true);
						sendVotes(getVotesArray());
						updatePollOptions(getPollNames());
						// setPollStarted();
						setPollVisible(true);
						break;
					}
				case 'vote_ended':
					{
						setPollVisible(true);
						sendVotes(getVotesArray());
						setVotingActive(false);
    					const winnerData = getWInnerData();

						updateWinner(winnerData.index);

						const effect = getWinnerEffect(winnerData.id);

						reactivateDisabledEffects();

						console.log(`Winner is ${effect ? effect.id : effect}`);

						if (effect)
						{
							tempDisableEffect(effect);

							gameWebSocketClient.send(JSON.stringify({ 
								type: "activate-effect", 
								id: effect.id,
								name: effect.name,
								duration: effect.duration ? effect.duration : 0 
						 	}));
						}

						setPollFadeOut();

						break;
					}
				default:
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

function sendVotes(votes: Array<number>)
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({ type: 'update-votes', data: votes }));
		}
	}
}

function updatePollOptions(options: Array<string>)
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({ type: 'new-options', data: options }));
		}
	}
}

function updateWinner(winner: number)
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({ type: 'set-winner', data: winner }));
		}
	}
}


function setPollStarted()
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({ type: 'poll-started' }));
		}
	}
}

function setPollVisible(bVisible: boolean)
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({ type: 'set-poll-visible', data: bVisible }));
		}
	}
}

function setPollFadeOut()
{
	if (overlayServer)
	{
		for (let ws of overlayServer.clients)
		{
			ws.send(JSON.stringify({ type: 'set-poll-fade-out' }));
		}
	}
}

export function startWSServer()
{
	overlayServer = new WebSocketServer({ port: 9147 });

    // setVotingActive(true);
    // setRandomPollOptions();
    
    overlayServer.on('connection', (ws) => {

		overlayClients.push(ws);
        let votesInterval: NodeJS.Timer | null = null;

        ws.on('message', function message(data) {
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
			ws.send(JSON.stringify({ type: 'update-votes', data: votes }));
		}

		votesInterval = setInterval(() =>
		{
			if (IsVotingEnabled())
			{
				sendVotes(getVotesArray());
			}
		}, 1000);

        ws.send(JSON.stringify({ type: 'new-options', data: getPollNames() }));

    });
}


