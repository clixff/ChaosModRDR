import tmi from 'tmi.js';
import {WebSocket} from "ws";
import {getConfig} from './config';
import {newVote} from './voting';
import axios from 'axios';
import {randomInteger} from '.';

let client: tmi.Client | null = null;

interface ITwitchProfile
{
	error: undefined | true,
	data: [{
		id: number,
		login: string,
		display_name: string,
		profile_image_url: string
	}];
}

export let chatUsernames: Array<string> = [];


export async function getTwitchUser(): Promise<string | null>
{
	try
	{
		const conf = getConfig();
		
		if (!conf.token)
		{
			return null;
		}
		
		const fetchedData = await axios.get('https://api.twitch.tv/helix/users', {
			headers: {
				'Client-ID': 'sdyeqfly1o09e63sfohhbcfuptzdu2',
				'Authorization': `Bearer ${conf.token}`
			},
			responseType: 'json'
		});
		
		const jsonData = (await fetchedData.data) as ITwitchProfile;
		
		if (jsonData && !jsonData.error)
		{
			const profile = jsonData.data[0];
			return profile.login;
		}
		
	}
	catch (err)
	{
		console.error(err);
	}
	
	return null;
}

export function handleSub(ws: WebSocket, channel: string, username: string, num_subs: number): void
{
	if (ws !== null)
	{
		ws.send(JSON.stringify({
			type: "subscribe-event",
			channel: channel,
			username: username,
			num_subs: num_subs
		}));
	}
}

export function startListeningChat(login: string, ws_provider: Function): void
{
	if (client)
	{
		client.disconnect();
		client = null;
	}
	
	const conf = getConfig();
	
	if (!conf.token)
	{
		return;
	}
	
	client = new tmi.Client({
		connection:
			{
				reconnect: true,
				secure: true
			},
		identity:
			{
				username: login,
				password: conf.token,
			},
		channels:
			[
				login
			]
	});
	
	client.connect();
	
	client.on("giftpaidupgrade", (channel: string, username: string, sender: string, userstate: any) =>
	{
		handleSub(ws_provider(), channel, username, 1);
	});
	
	client.on("resub", (channel: string, username: string, months: number, message: string, userstate: any, methods: any) =>
	{
		handleSub(ws_provider(), channel, username, 1);
	});
	
	client.on("subgift", (channel: string, username: string, months: number, recipient: string, methods: any, userstate: any) =>
	{
		handleSub(ws_provider(), channel, username, 1);
	});
	
	/*
	client.on("submysterygift", (channel: string, username: string, num_subs: number, methods: any, userstate: any) => {
		handleSub(ws_provider(), channel, username, num_subs);
	});
	 */
	
	client.on("subscription", (channel: string, username: string, method: any, message: string, userstate: any) =>
	{
		handleSub(ws_provider(), channel, username, 1)
	});
	
	client.on('message', (target: string, context: any, msg: string, self: boolean) =>
	{
		msg = msg.trim();
		
		if (!context)
		{
			context = {};
		}
		
		if (context['message-type'] === 'chat' && context['user-id'])
		{
			let num = Number(msg);
			
			if (isFinite(num) && num >= 1 && num <= 8)
			{
				if (num >= 5)
				{
					num -= 4;
				}
				
				newVote(Math.floor(num) - 1, context['user-id']);
				
				const displayName = context['display-name'] || '';
				const username = context.username || '';
				
				const name = username == displayName.toLowerCase() ? displayName : username;
				
				if (!chatUsernames.includes(name))
				{
					chatUsernames.push(name);
					
					if (chatUsernames.length >= 50)
					{
						chatUsernames.splice(0, 1);
					}
				}
			}
		}
	});
	
	client.on('connected', (addr: string, port: number) =>
	{
		console.log(`[Twitch] Connected to ${addr}:${port}`)
	});
}

export function getRandomTwitchNickname(): string
{
	if (!chatUsernames.length)
	{
		return "";
	}
	
	return chatUsernames[randomInteger(0, chatUsernames.length - 1)];
}

export function clearChatUsernames(): void
{
	chatUsernames = [];
}