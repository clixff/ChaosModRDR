import { promises as fsPromises } from 'fs';

export interface IConfig
{
    /** Twitch auth token */
    token: string
};

let _config: IConfig = {
    token: "",
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