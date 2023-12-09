#pragma once

#define HELPSTRING "\nStatic HTTP server\n"\
	"Usage: %s [-p port number] [-h html directory] [-t thread pool size]\n"\
	"\n[Optional arguments]\n"\
	"\t-p\tServer port (Default 8000)\n"\
	"\t-t\tThread pool size. Number of threads for the server to use. (Default 8)\n"\
	"\t-s\tHTML files in this directory are served. This directory must be in the same directory\n"\
	"\t\tas the server binary and don't add './' to the directory name! (Default 'html')\n"\
	"\t-h\tShows available arguments\n"

#define HTTP_BASE_OK "HTTP/1.1 200 OK\r\n"\
	"Server: Single File Server\r\n"\
	"Connection: Closed\r\n"\
	"Content-Type: "

#define HTTP_404 "HTTP/1.1 404 Not Found\r\n"\
	"Server: Single File Server\r\n"\
	"Content-Type: text/html; charset=utf-8\r\n"\
	"Connection: Closed\r\n\r\nPAGE NOT FOUND"

#define HTTP_405 "HTTP/1.1 405 Method Not Allowed\r\n"\
	"Server: Single File Server\r\n"\
	"Content-Type: text/html; charset=utf-8\r\n"\
	"Connection: Closed\r\n\r\nONLY GET AND HEAD ARE ACCEPTED"

#define HTTP_BASE_OK_len strlen(HTTP_BASE_OK)
#define HTTP_404_len strlen(HTTP_404)
#define HTTP_405_len strlen(HTTP_405)


enum filetype
{
	HTML,
	CSS,
	JS,
	JPG,
	JPEG,
	SWF,
	GIF,
	PNG,
	TXT,
	SVG,
	UNKNOWN
};

enum http_status
{
	OK,
	INCOMPLETE,
	NOTALLOWED,
	BADFILE,
	NOTFOUND
};
