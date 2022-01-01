import { getConfig, readConfig } from "./config"; 
import { startHTTPServer } from "./server";
import { connectWebsocketClient, startWSServer } from "./sockets";
import { getTwitchUser, startListeningChat } from "./twitch";

export function randomInteger(min: number, max: number): number
{
    const rand = min - 0.5 + Math.random() * (max - min + 1);
    return Math.round(rand);
}

async function Init(): Promise<void>
{
    await readConfig();
    startHTTPServer();
    startWSServer();

    const login = await getTwitchUser();

    if (login)
    {
        startListeningChat(login);
    }

    connectWebsocketClient();
};

Init();
