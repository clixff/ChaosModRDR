import { randomInteger } from ".";
import { IEffect } from "./config";

interface IEffectVotingData 
{
    id: string,
    name: string,
    /** Array of twitch user ids */
    votes: Array<string>
}


const poll: [IEffectVotingData, IEffectVotingData, IEffectVotingData, IEffectVotingData] = [
    {
        id: '',
        name: '',
        votes: []
    },
    {
        id: '',
        name: '',
        votes: []
    },
    {
        id: '',
        name: '',
        votes: []
    },
    {
        id: '',
        name: '',
        votes: []
    }
];

let bVotingActive = false;

let effectsList: Array<IEffect> = [];
let effectsMap = new Map<string, IEffect>();

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


export function getWinnerIDByVotes(votes: Array<number>): number
{
    const total = votes.reduce((prev, cur) => { return prev + cur; });
    
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

export function getWInnerData(): { id: string, index: number }
{
    let arrVotes = [];

    for (let i = 0; i < poll.length; i++)
    {
        arrVotes.push(poll[i].votes.length);
    }

    const winnerId = getWinnerIDByVotes(arrVotes);

    return {
        id: poll[winnerId].id,
        index: winnerId
    };
}

export function setVotingActive(value: boolean): void
{
    bVotingActive = value;
};

export function setRandomPollOptions(): void
{
    const pool = [...effectsList];
    
    for (let i = 0; i < 3; i++)
    {
        const index = randomInteger(0, pool.length - 1);
        const effect = pool[index];
        poll[i].id = effect.id;    
        poll[i].name = effect.name;
        poll[i].votes = [];
        pool.splice(index, 1);
    }

    poll[3].id = 'random_effect';
    poll[3].name = 'Random Effect';
    poll[3].votes = [];
}

export function createEnabledEffectsList(allEffects: Array<IEffect>): void
{
    effectsList = [];
    effectsMap.clear();
    for (let i = 0; i < allEffects.length; i++)
    {
        if (allEffects[i].enabled)
        {
            effectsList.push(allEffects[i]);
            effectsMap.set(allEffects[i].id, allEffects[i]);
        }
    }
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

export function getWinnerEffect(effectID: string): IEffect | null
{
    if (effectID === 'random_effect')
    {
        return effectsList[randomInteger(0, effectsList.length - 1)];
    }
    
    const effect = effectsMap.get(effectID);

    if (effect)
    {
        return effect;
    }

    return null;
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
        poll[i].id = "";    
        poll[i].name = `Effect ${i+1}`;
        poll[i].votes = [];
    }

    poll[3].id = 'random_effect';
    poll[3].name = 'Random Effect';
    poll[3].votes = [];
}