import Fastify from "fastify";
import path from 'path';
import fs from 'fs';
import {updateToken} from "./config";

const fastify = Fastify({
	logger: false
});

fastify.get('/', (req, reply) =>
{
	const stream = fs.createReadStream(path.join(__dirname, '../', 'front/index.html'));
	reply.type('text/html').send(stream);
});

fastify.get('/index.js', (req, reply) =>
{
	const stream = fs.createReadStream(path.join(__dirname, '../', 'front/index.js'));
	reply.type('text/javascript').send(stream);
});

fastify.get('/style.css', (req, reply) =>
{
	const stream = fs.createReadStream(path.join(__dirname, '../', 'front/style.css'));
	reply.type('text/css').send(stream);
});

fastify.get('/login', (req, reply) =>
{
	const url = `https://id.twitch.tv/oauth2/authorize?response_type=token&client_id=sdyeqfly1o09e63sfohhbcfuptzdu2&redirect_uri=http://localhost:9148/token&scope=chat:read`;
	reply.redirect(url);
});

fastify.get('/token', (req, reply) =>
{
	const stream = fs.createReadStream(path.join(__dirname, '../', 'front/token.html'));
	reply.type('text/html').send(stream);
});

fastify.post('/token-update', (req, reply) =>
{
	reply.send('OK');
	
	console.log(req.query);
	
	if (req.query)
	{
		const query = req.query as Record<string, string>;
		
		if (query['token'] && query['token'].length)
		{
			updateToken(query['token']);
		}
	}
});

export function startHTTPServer(bLocalIP: boolean = true): void
{
	const ip = bLocalIP ? '127.0.0.1' : '0.0.0.0';
	fastify.listen({port: 9148, host: ip}, (err, addr) =>
	{
		if (err)
		{
			throw err;
		}
		
		console.log('[Server] Server is listening on ' + addr);
	});
}