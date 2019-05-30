# RoboPlay
RoboPlay is a multiformat music replayer for the MSX platform for the Yamaha OPL3/4 soundchip. It runs only under MSX-DOS2.

It is written in C using [SDCC](http://sdcc.sourceforge.net/) and the [FUSION-C](https://www.msx.org/news/development/en/fusion-c-is-available) library.

Currently the following file formats are supported:
- SOP: Note OPL3 Music Sequencer 
- D00: EdLib packed module
- DRO: DOSBox Raw OPL v2.0
- RAW: DosBOX RAW OPL capture

# Installation
RoboPlay consists of the main RoboPlay application and a separate player plugin (.PLY) per supported file format.

The main application together with all player files can be placed on any folder location, just make sure all players are located in the same folder. Add the location of the main application to the search path to be able to start it from an

The environment variable ROBOPLAY is used to set to location of the players. When for example the RoboPlay folder is located in A:\ROBOPLAY use the following:
> SET ROBOPLAY = A:\ROBOPLAY\

# Usage
To start the player, start the main application followed by optionally the name of the player file and the song file to play.

> **ROBOPLAY** [player.PLY] song.EXT
The player name is optional, when no player is provided the file extension of the song is used for selecting which player to use
For eample:
- **ROBOPLAY SOP.PLY TEST.SOP** will play the song TEST.SOP using SOP.PLY as the player.
- **ROBOPLAY TEST.SOP** will play the song TEST.SOP using SOP.PLY as the player
- **ROBOPLAY SOP.PLY TEST.RAW** will play the song TEST.RAW using SOP.PLY as the player

During replay use **ESC** to stop replaying. When the song contains multiple sub-songs, use **LEFT** and **RIGHT** to go to previous or next subsong.

# Players

## ROBO_PLAYER_INTERFACE
All players use the same header in which the ROBO_PLAYER_INTERFACE is declared. This interface is used to plug-in the player into the main application

The following methods need to implemented by a player and are called from the RoboPlay framework:
>* *boolean RP_Load(char** *fileName)*
>Called when a song file needs to be loaded
>* *boolean RP_Update()*
>Called every interrupt cycle to play the next song event
>* *void RP_Rewind(int8_t subsong)*
>Reset the song to it's initial state and select the provided subsong (when applicable)
>* *float RP_GetRefresh();*
>Provide the frequency (in Hz) of the RP_Update calls

>* *uint8_t RP_GetSubSongs()*
> Return the number of sub-songs. 

>* *char** *RP_GetPlayerInfo()*
> Return information about the player
>* *char** *RP_GetTitle()*
> Return the title of the song
>* *char** *RP_GetAuthor()*
> Return the author of the song
>* *char** *RP_GetDescription()*
> Return the description of the song

Next to this the interface also provides a number of methods towards the RoboPlay framework which can be used by the player:
>* *RP_Open(char** *name)*
>Open a file
>* *uint_16_t RP_Read(void* *buf, uint16_t nbytes)*
>Read *nbytes* bytes from the file opened by RP_Open. Returns the actual number of bytes read
>* *void RP_Close()*
>Close the file opened by RP_Open
>* *boolean RP_Exists(char* *name)*
>* Check if a specific file exists

>* *uint8_t RP_AllocateSegment()*
>Allocate a new memory segment. Segments are always a block of 16KB and always located on Page 2 (*0x8000 - 0xBFFF*). Returns the index number of the allocated segment
>* *void RP_SetSegment(uint8_t segment)*
>Set an allocated segment active on Page 2

>* *void RP_UpdateRefresh()*
>Force a RP_GetRefresh call from the framework to set a new refresh rate.

>* *RP_WriteOpl1(uint8_t addr, uint8_t value)*
>Write value *value* to register *addr* using the first OPL port (*0xC4*)
>* *RP_WriteOpl2(uint8_t addr, uint8_t value)*
>Write value *value* to register *addr* using the second OPL port (*0xC6*)

## Makefile
When a new player is added, place it in the *players* subfolder and add the name of the player to the line starting with *all*:
> all: $(TARGETNAME).com dro.ply sop.ply d00.ply raw.ply clean

All players are linked together with the same assembly file (*player.s*) which provides the binary player headers as well as the entry points used for the ROBO_PLAYER_INTERFACE. 
When one or more of the mandatory interface methods are not impmented by the player this will result in link errors, acting sort of as a virtual base class.

## Memory layout
* Main application of the framework uses Page 0 (*0x000 - 0x3FFF*)
* Players are located starting from Page 1 (*0x4000 - 0x7FFF*)
* Song data can be freely located on every location in Page 1 or higher not yet in use. When more memory is needed this can be allocated using *RP_AllocateSegment* and set using *RP_SetSegment*. This will **always** be on Page 2.