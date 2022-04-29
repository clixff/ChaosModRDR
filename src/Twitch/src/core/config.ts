import {promises as fsPromises} from 'fs';

export interface IConfig
{
	/** Twitch auth token */
	token: string;
	/** Use 127.0.0.1 for internal resources when true.  */
	local_ip: boolean;
	max_options: 4 | 8;
	weighted_voting: boolean;
}

let _config: IConfig = {
	token: "",
	local_ip: true,
	max_options: 4,
	weighted_voting: false
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
		const fileData = await fsPromises.readFile(configPath, {encoding: 'utf-8'});
		
		if (fileData)
		{
			_config = JSON.parse(fileData);
			
			if (_config.max_options != 4 && _config.max_options != 8)
			{
				_config.max_options = 4;
			}
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
		await fsPromises.writeFile(configPath, JSON.stringify(_config, null, '\t'), {encoding: 'utf-8'});
	}
	catch (err)
	{
		console.log(err);
	}
}

export async function updateToken(newToken: string): Promise<void>
{
	_config.token = newToken;
	await saveConfig();
}