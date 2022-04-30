interface IEffectVotingData
{
	name: string,
	/** Array of twitch user ids */
	votes: Array<string>
}


const poll: [IEffectVotingData, IEffectVotingData, IEffectVotingData, IEffectVotingData] = [
	{
		name: '',
		votes: []
	},
	{
		name: '',
		votes: []
	},
	{
		name: '',
		votes: []
	},
	{
		name: '',
		votes: []
	}
];

let bVotingActive = false;

export function newVote(optionNum: number, userID: string)
{
	if (!bVotingActive)
	{
		return;
	}
	
	for (let i = 0; i < poll.length; i++)
	{
		const index = poll[i].votes.indexOf(userID);
		if (index !== -1)
		{
			poll[i].votes.splice(index, 1);
		}
	}
	
	if (poll[optionNum])
	{
		poll[optionNum].votes.push(userID);
	}
}

export function getWinnerIDByVotes(votes: Array<number>, weighted_random: boolean): number
{
	if (!weighted_random)
	{
		const total = votes.reduce((prev, cur) =>
		{
			return prev + cur;
		});
		
		/** Return random effect if no votes */
		if (!total)
		{
			return votes.length - 1;
		}
		
		let max = 0;
		let indexMax = 0;
		
		for (let i = 0; i < votes.length; i++)
		{
			if (votes[i] > max)
			{
				indexMax = i;
				max = votes[i];
			}
		}
		return indexMax;
	}
	
	let i: number;
	let weights: number[] = [];
	
	for (i = 0; i < votes.length; i++)
	{
		weights[i] = votes[i] + (weights[i - 1] || 0);
	}
	
	let rand = Math.random() * weights[weights.length - 1];
	
	for (i = 0; i < weights.length; i++)
	{
		if (weights[i] > rand)
		{
			break;
		}
	}
	
	return i;
}

export function getWinnerIndex(weighted_random: boolean): number
{
	let arrVotes = [];
	
	for (let i = 0; i < poll.length; i++)
	{
		arrVotes.push(poll[i].votes.length);
	}
	
	return getWinnerIDByVotes(arrVotes, weighted_random);
}

export function setVotingActive(value: boolean): void
{
	bVotingActive = value;
}

export function getPollNames(): Array<string>
{
	const names: Array<string> = [];
	for (let i = 0; i < poll.length; i++)
	{
		names.push(poll[i].name);
	}
	
	return names;
}

export function getVotesArray(): Array<number>
{
	const arrVotes = [];
	
	for (let i = 0; i < poll.length; i++)
	{
		arrVotes.push(poll[i].votes.length);
	}
	return arrVotes;
}

export function IsVotingEnabled()
{
	return bVotingActive;
}

export function resetPoll(): void
{
	for (let i = 0; i < 3; i++)
	{
		poll[i].name = `Effect ${i + 1}`;
		poll[i].votes = [];
	}
	
	poll[3].name = 'Random Effect';
	poll[3].votes = [];
}

export function updateEffectNamesFromGame(names: Array<string>)
{
	for (let i = 0; i < poll.length; i++)
	{
		poll[i].name = names[i];
		poll[i].votes = [];
	}
}