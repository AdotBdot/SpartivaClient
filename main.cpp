#include "Client.h"

#include <string>

int main( int argc, char **argv )
{
	Client c( argc,*argv );
	c.run( );

	return 0;
}