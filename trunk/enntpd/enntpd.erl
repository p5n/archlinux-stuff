-module(enntpd).
-export([start/0]).

-define(TCP_OPTIONS,[list, {packet, 0}, {active, false}, {reuseaddr, true}]).

start() ->
    listen(5000).

listen(Port) ->
    {ok, LSocket} = gen_tcp:listen(Port, ?TCP_OPTIONS),
    do_accept(LSocket).

do_accept(LSocket) ->
    case gen_tcp:accept(LSocket) of
        {ok, Socket} ->
            spawn(fun() -> handle_client(Socket) end);
        {error, Reason} ->
            io:format("Socket accept error: ~s~n", [Reason])
    end,
    do_accept(LSocket).

handle_request(["LIST"]) ->
    io:format("LIST COMMAND~n");
handle_request([H|T]) ->
    io:format("OTHER COMMAND ~s  -  ~s~n", [H, T]).

handle_client(Socket) ->
    case gen_tcp:recv(Socket, 0) of
        {ok, Data} ->
            io:format("Data: ~w:~s~n", [Socket, Data]),
            Cmds = string:tokens(Data, "\r\n"),
            handle_request(Cmds),
            gen_tcp:send(Socket, Data),
            handle_client(Socket);
        {error, closed} ->
            io:format("Disconnect~n")
    end.
