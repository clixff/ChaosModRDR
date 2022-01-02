import { promises as fsPromises } from 'fs';
import { createEnabledEffectsList } from './voting';


export interface IEffect
{
    id: string,
    name: string,
    enabled: boolean;
    duration?: number;
    chance?: number;
};

export interface IConfig
{
    /** Timer interval in seconds */
    interval: number,
    votingDuration: number,

    /** Twitch auth token */
    token: string,
    effects: Array<IEffect>;
};

let _config: IConfig = {
    interval: 90,
    votingDuration: 45,
    token: "",
    effects: []
};

let configPath = 'config.json';

export function getConfig(): IConfig 
{
    return _config;
}

export async function readConfig(): Promise<void>
{
    try
    {
        const fileData = await fsPromises.readFile(configPath, { encoding: 'utf-8' });

        if (fileData)
        {
            _config = JSON.parse(fileData);

            createEnabledEffectsList(_config.effects);
        }
    }
    catch (err)
    {
        console.log(err);
    }
}

export async function saveConfig(): Promise<void>
{
    try
    {
        await fsPromises.writeFile(configPath, JSON.stringify(_config, null, '\t'), { encoding: 'utf-8' });
    }
    catch (err)
    {
        console.log(err);
    }
};

export async function updateToken(newToken: string): Promise<void>
{
    _config.token = newToken;
    await saveConfig();
}