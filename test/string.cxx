#include <catch.hpp>
#define ProgramOptions_silent
#include <ProgramOptions.hxx>
#include "arg_provider.hxx"

TEST_CASE( "string", "[ProgramOptions]" ) {
	po::parser parser;
	auto&& a = parser[ "a" ]
		.type( po::string );
	auto&& b = parser[ "bot" ]
		.abbreviation( 'b' )
		.type( po::string )
		.multi();
	auto&& c = parser[ "c" ]
		.type( po::string )
		.fallback( "test" );
	auto&& f = parser[ "f" ]
		.type( po::string )
		.multi()
		.fallback( 3, "no-exceptions" );

	REQUIRE( a.count() == 0 );
	REQUIRE( b.count() == 0 );
	REQUIRE( c.count() == 1 );
	REQUIRE( c.get().string == "test" );
	REQUIRE( f.count() == 2 );
	REQUIRE( f.get( 0 ).string == "3" );
	REQUIRE( f.get( 1 ).string == "no-exceptions" );

	SECTION( "Scenario 1" ) {
		const arg_provider A {
			"/Test",
			"-a text",
			"-a/path/to/nowhere",
			"--bot=net",
			"--bot text",
			"-beast",
			"--bot",
			"whatever",
			"-c=/usr/bin",
			"-f",
			"-f",
			"down here",
			"-flto",
			"-fmax-errors=25",
			"-fno-rtti"
		};
		REQUIRE( parser( A.argc, A.argv ) );
		REQUIRE( a.count() == 1 );
		CHECK( a.get().string == "/path/to/nowhere" );
		REQUIRE( b.count() == 4 );
		CHECK( b.get( 0 ).string == "net" );
		CHECK( b.get( 1 ).string == " text" );
		CHECK( b.get( 2 ).string == "east" );
		CHECK( b.get( 3 ).string == "whatever" );
		REQUIRE( c.count() == 1 );
		CHECK( c.get().string == "/usr/bin" );
		REQUIRE( f.count() == 5 );
		CHECK( f.get( 0 ).string == "" );
		CHECK( f.get( 1 ).string == "down here" );
		CHECK( f.get( 2 ).string == "lto" );
		CHECK( f.get( 3 ).string == "max-errors=25" );
		CHECK( f.get( 4 ).string == "no-rtti" );
	}
}