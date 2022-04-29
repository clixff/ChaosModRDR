import {getConfig, readConfig} from "./config";
import {startHTTPServer} from "./server";
import {connectWebsocketClient, getWebSocket, startWSServer} from "./sockets";
import {getTwitchUser, startListeningChat} from "./twitch";

export function randomInteger(min: number, max: number): number
{
	const rand = min - 0.5 + Math.random() * (max - min + 1);
	return Math.round(rand);
}

async function Init(): Promise<void>
{
	await readConfig();
	const _conf = getConfig();
	const bUseLocalHost = _conf.local_ip != undefined ? _conf.local_ip : true;
	startHTTPServer(bUseLocalHost);
	startWSServer(bUseLocalHost);
	
	const login = await getTwitchUser();
	
	if (login)
	{
		startListeningChat(login, getWebSocket);
	}
	
	connectWebsocketClient();
}

Init();
