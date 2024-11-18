#include "GameManager.h"

namespace SDLFramework {
    GameManager* GameManager::sInstance = nullptr;

    GameManager* GameManager::Instance() {
        if (sInstance == nullptr) {
            sInstance = new GameManager();
        }

        return sInstance;
    }

    void GameManager::Release() {
        delete sInstance;
        sInstance = nullptr;
    }

    void GameManager::Run() {
        //Main Game Loop
        while (!mQuit) {
            mTimer->Update();
            //Event Polling Loop
            //While there are events inside of our events varaible...
            while (SDL_PollEvent(&mEvents)) {
                //Handle each and every event we want to look for
                switch (mEvents.type) {
                case SDL_QUIT:
                    mQuit = true;
                    break;
                }
            }

            if (mTimer->DeltaTime() >= 1.0f / FRAME_RATE) {
                mTimer->Reset();
                Update();
                LateUpdate();
                Render();
            }
        }
    }

    void GameManager::Update() {
        mInputManager->Update();

        if (mInputManager->KeyDown(SDL_SCANCODE_W)) {
            mTex->Translate(Vector2(0, -40.0f) * mTimer->DeltaTime(), GameEntity::World);
        }
        else if (mInputManager->KeyDown(SDL_SCANCODE_S)) {
            mTex->Translate(Vector2(0, 40.0f) * mTimer->DeltaTime(), GameEntity::World);
        }
        //To prevent diagonal movement, add an else to the if statement below
        if (mInputManager->KeyDown(SDL_SCANCODE_A)) {
            mTex->Translate(Vector2(-40.0f, 0.0f) * mTimer->DeltaTime(), GameEntity::World);
        }
        else if (mInputManager->KeyDown(SDL_SCANCODE_D)) {
            mTex->Translate(Vector2(40.0f, 0.0f) * mTimer->DeltaTime(), GameEntity::World);
        }

        //TODO:
        //Setup rotation using the Q and E keys
        //Setup scaling the texture using the Z and C keys
        //Setup a second set of movement (ie arrow keys/IJKL) for the second texture

        if (mInputManager->KeyPressed(SDL_SCANCODE_SPACE)) {
            //std::cout << "Space key pressed!" << std::endl;
            mAudioManager->PlaySFX("Audio/SFX/coin_credit.wav", 0, -1);
        }

        if (mInputManager->KeyReleased(SDL_SCANCODE_SPACE)) {
            std::cout << "Space key released!" << std::endl;
        }

        if (mInputManager->MouseButtonPressed(InputManager::Left)) {
            std::cout << "Left Mouse button pressed!" << std::endl;
        }

        if (mInputManager->MouseButtonReleased(InputManager::Left)) {
            std::cout << "Left Mouse button released!" << std::endl;
        }

        mTex->Update();
    }

    void GameManager::LateUpdate() {
        mInputManager->UpdatePrevInput();
    }

    void GameManager::Render() {
        //This is the old frame we need to clear
        mGraphics->ClearBackBuffer();

        mTex->Render();
        mFontTex->Render();

        //Actually showing everthing that we have told to render
        mGraphics->Render();
    }

    GameManager::GameManager() : mQuit(false) {
        //calling to our Graphics Singleton
        mGraphics = Graphics::Instance();

        if (!Graphics::Initialized()) {
            mQuit = true;
        }

        //Initialize all other modules
        mTimer = Timer::Instance();
        mAssetManager = AssetManager::Instance();
        mInputManager = InputManager::Instance();
        mAudioManager = AudioManager::Instance();

        mTex = new AnimatedTexture("SpriteSheet.png", 204, 45, 40, 38, 4, 0.5f, AnimatedTexture::Horizontal);
        mTex->Scale(Vector2(1.5f, 1.5f));

        mTex->Position(Vector2(Graphics::SCREEN_WIDTH * 0.49f, Graphics::SCREEN_HEIGHT * 0.5f));

        mFontTex = new Texture("Hello World!", "ARCADE.TTF", 72, { 255, 0, 0 });
        mFontTex->Position(Vector2(400, 200));
    }

    GameManager::~GameManager() {
        //Release Variables
        delete mTex;
        mTex = nullptr;

        delete mFontTex;
        mFontTex = nullptr;

        //Release Modules
        Graphics::Release();
        mGraphics = nullptr;

        Timer::Release();
        mTimer = nullptr;

        AssetManager::Release();
        mAssetManager = nullptr;

        InputManager::Release();
        mInputManager = nullptr;

        AudioManager::Release();
        mAudioManager = nullptr;

        //Quit SDl Subsystems
        SDL_Quit();
    }
    //Namespace bracket is below
}