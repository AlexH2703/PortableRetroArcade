#include "MyForm.h"
#include "TransparentTextBox.h"
//#include <SDL.h>
//#include <SDL_mixer.h>
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void main(array<System::String^>^ args) {
    /*
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return; // Exit if SDL initialization fails+9
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return; // Exit if SDL_mixer initialization fails
    }

    // Load the MP3 file
    Mix_Music* music = Mix_LoadMUS("C:\\Users\\grace\\OneDrive\\Documents\\Senior Design\\Bit_Shift_Song.mp3");
    if (!music) {
        std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        Mix_CloseAudio();
        SDL_Quit();
        return; // Exit if music loading fails
    }

    // Play the music
    Mix_PlayMusic(music, -1); // -1 means loop indefinitely
    */
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    PortableArcadeUI::MyForm form;

    Application::Run(% form); // Start the Windows Forms application
    /*
    // Clean up after the application closes
    Mix_FreeMusic(music); // Free the music
    Mix_CloseAudio(); // Close the audio subsystem
    SDL_Quit(); // Quit SDL
    */
}
