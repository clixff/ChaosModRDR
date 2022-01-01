import tmi from 'tmi.js';
import { getConfig } from './config';
import { newVote } from './voting';
import axios from 'axios';

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


export function startListeningChat(login: string): void
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

    client.on('message', (target, context, msg, self) =>
    {
        msg = msg.trim();

        if (!context)
        {
            context = {};
        }

        if (context['message-type'] === 'chat' && context['user-id'])
        {
            const num = Number(msg);

            if (isFinite(num) && num >= 1 && num <= 4)
            {
                newVote(Math.floor(num) -  1, context['user-id']);
            }
        }
    });

    client.on('connected', (addr, port) => {
		console.log(`[Twitch] Connected to ${addr}:${port}`)
	});
}