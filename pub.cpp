#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis.h>

int main(int argc, char **argv) 
{
    char server[1024];
    redisContext *ctx;
    redisReply   *reply;
    
    // Check command line
    if (argc > 1)
    {
       snprintf(server, sizeof(server), "%s", argv[1]);
    }
    else
    {
       snprintf(server, sizeof(server), "127.0.0.1");
    }

    // Set up for the run
    int port = 6379;
    struct timeval timeout = { 1, 0 }; // 1.0 seconds

    // Connect to the server
    ctx = redisConnectWithTimeout(server, port, timeout);
    if (ctx == NULL || ctx->err) {
        if (ctx) {
            printf("Connection error: %s", ctx->errstr);
            redisFree(ctx);
        } else {
            printf("Connection error: can't allocate redis context");
        }
        printf(" ... exiting\n");
        exit(1);
    }

    /* PING server */
    reply = (redisReply *)redisCommand(ctx,"PING");
    printf("PING: %s\n", reply->str);
    freeReplyObject(reply);

    /* Set a key */
    reply = (redisReply *)redisCommand(ctx,"SET %s %s", "foo", "hello world");
    printf("SET: %s\n", reply->str);
    freeReplyObject(reply);

    /* Set a key using binary safe API */
    reply = (redisReply *)redisCommand(ctx,"SET %b %b", "bar", (size_t) 3, "hello", (size_t) 5);
    printf("SET (binary API): %s\n", reply->str);
    freeReplyObject(reply);

    /* Try a GET and two INCR */
    reply = (redisReply *)redisCommand(ctx,"GET foo");
    printf("GET foo: %s\n", reply->str);
    freeReplyObject(reply);

    reply = (redisReply *)redisCommand(ctx,"INCR counter");
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);
    /* again ... */
    reply = (redisReply *)redisCommand(ctx,"INCR counter");
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);

    /* Create a list of numbers, from 0 to 9 */
    reply = (redisReply *)redisCommand(ctx,"DEL mylist");
    freeReplyObject(reply);
    for (int ii = 0; ii < 10; ii++) {
        char buf[64];

        snprintf(buf,64,"%u",ii);
        reply = (redisReply *)redisCommand(ctx,"LPUSH mylist element-%s", buf);
        freeReplyObject(reply);
    }

    /* Let's check what we have inside the list */
    reply = (redisReply *)redisCommand(ctx,"LRANGE mylist 0 -1");
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (int ii = 0; ii < reply->elements; ii++) {
            printf("%u) %s\n", ii, reply->element[ii]->str);
        }
    }
    freeReplyObject(reply);

    /* Disconnects and frees the context */
    redisFree(ctx);

    return 0;
}
