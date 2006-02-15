#include <stdio.h>
#include <string.h>

#include <apps/shell/tinysh.h>

static volatile int shell_canquit = 0;

static volatile char * shell_pworkingdir;
char shell_workingdir[PROMPT_SIZE];

void tinysh_char_out( unsigned char c )
{
	putchar( (int)c );
}

static void display_args(int argc, char **argv)
{
  int i;
  for(i=0;i<argc;i++)
    {
      printf("argv[%d]=\"%s\"\n",i,argv[i]);
    }
}

static void shell_quit( int argc, char **argv )
{
	printf( "quit: quitting...\n" );
	shell_canquit = 1;
}

static void shell_create( int argc, char **argv )
{
display_args(argc,argv);
}

static void shell_delete( int argc, char **argv )
{
display_args(argc,argv);
}

static void shell_rename( int argc, char **argv )
{
display_args(argc,argv);
}

static void shell_copy( int argc, char **argv )
{
display_args(argc,argv);
}

static void shell_list( int argc, char **argv )
{
display_args(argc,argv);
}

static void shell_cd( int argc, char **argv )
{
display_args(argc,argv);
}

static void shell_mount( int argc, char **argv )
{
display_args(argc,argv);
}

static void shell_unmount( int argc, char **argv )
{
display_args(argc,argv);
}

static void shell_spawn( int argc, char **argv )
{
	int wait = 1;
	int pid = 0;
	
	if( argc < 1 )
	{
		printf("spawn: you must enter an executable filename to spawn.\n");
		return;
	}
	
	if( strcmp( argv[argc-1], "&" ) == 0 )
		wait = 0;
		
	printf("spawn: amos_spawn( %s )\n", argv[1] );
	
	/*
	pid = amos_spawn( argv[1] );
	if( wait )
		amos_wait( pid );
	*/
}

static void shell_kill( int argc, char **argv )
{
	int pid = 0;

	if( argc < 1 )
	{
		printf("kill: you must enter a process id to kill.\n");
		return;
	}
	
	pid = atoi( argv[1] );
	if( pid == 0 )
	{
		printf("kill: you must enter a valid process id.\n");
		return;
	}
	
	printf("kill: amos_kill( %d )\n", pid );
	//amos_kill( pid );
}

// create a directory
// delete a directory
static tinysh_cmd_t quitcmd    = { 0, "quit",    "exit the shell",       0, shell_quit,    0, 0, 0 };
static tinysh_cmd_t createcmd  = { 0, "create",  "create a file",    "[file]", shell_create,  0, 0, 0 };
static tinysh_cmd_t deletecmd  = { 0, "delete",  "delete a file",    "[file]", shell_delete,  0, 0, 0 };
static tinysh_cmd_t renamecmd  = { 0, "rename",  "rename a file",    "[source file] [destination file]", shell_rename,  0, 0, 0 };
static tinysh_cmd_t copycmd    = { 0, "copy",    "copy a file",      "[source file] [destination file]", shell_copy,    0, 0, 0 };
static tinysh_cmd_t listcmd    = { 0, "list",    "list directory contents", "<directory>", shell_list,    0, 0, 0 };
static tinysh_cmd_t cdcmd      = { 0, "cd",      "change working directory", "[directory]", shell_cd,    0, 0, 0 };
static tinysh_cmd_t mountcmd   = { 0, "mount",   "mount a volume",   "[device] [mountpoint] [file system]", shell_mount,   0, 0, 0 };
static tinysh_cmd_t unmountcmd = { 0, "unmount", "unmount a volume", "[mountpoint]", shell_unmount, 0, 0, 0 };
static tinysh_cmd_t spawncmd   = { 0, "spawn",   "spawn a process",  "[executable]", shell_spawn,   0, 0, 0 };
static tinysh_cmd_t killcmd    = { 0, "kill",    "kill a process",   "[process id]", shell_kill,    0, 0, 0 };

void shell_exit( void )
{
	// perform any tidy up here
	exit(0);
}

void shell_args( int argc, char **argv )
{
	int i;

	for( i=0 ; i<argc ; i++ )
	{
		if( strcmp( argv[i], "-h" ) == 0 )
		{
			printf("Shell Usage:\n");
			shell_exit();
		} else if( strcmp( argv[i], "-v" ) == 0 )
		{
			printf("Shell Version 1.0\n");
			shell_exit();
		}
	}
}

void shell_init( int argc, char **argv )
{
	char * p;

	// add the default commands
	tinysh_add_command( &quitcmd );    
	tinysh_add_command( &createcmd );
	tinysh_add_command( &deletecmd );
	tinysh_add_command( &renamecmd );
	tinysh_add_command( &copycmd );
	tinysh_add_command( &listcmd );
	tinysh_add_command( &cdcmd );
	tinysh_add_command( &mountcmd );
	tinysh_add_command( &unmountcmd );
	tinysh_add_command( &spawncmd );
	tinysh_add_command( &killcmd );
	
	// set the default working directory
	// chomp off the end file name
	p = strrchr( argv[0], '/' );
	if( p != NULL )
		*p = '\0';
	strcpy( shell_workingdir, argv[0] );
	shell_pworkingdir = &shell_workingdir;

	// process the arguments
	if( argc > 1 )
		shell_args( argc, argv );
}
 
int main( int argc, char **argv )
{
	char buffer[BUFFER_SIZE];
	char * pbuff;
	char prompt[PROMPT_SIZE];
	
	shell_init( argc, argv );
	
	sprintf( prompt, "AMOS:%s>", shell_pworkingdir );
	tinysh_set_prompt( prompt );
		
	while( !shell_canquit )
	{
		pbuff = &buffer;
		fgets( pbuff, BUFFER_SIZE, stdin );
		while( *pbuff )
			tinysh_char_in( *pbuff++ );
	}
	
	shell_exit();
}