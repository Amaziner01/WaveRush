#include "Game.hpp"

#include "Core/Timer.hpp"
#include "Scene/PlayScene.hpp"

Game::Game()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::exit(EXIT_FAILURE);
    }

    m_Window = SDL_CreateWindow(
            m_Settings.Title.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            m_Settings.Width, m_Settings.Height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (m_Window == nullptr) {
        std::exit(EXIT_FAILURE);
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

    if (m_Renderer == nullptr) {
        std::exit(EXIT_FAILURE);
    }

    m_Running = true;
}

Game::~Game()
{
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

Game& Game::Instance()
{
    static Game s_Instance;
    return s_Instance;
}

void Game::Run()
{
    // Init
    {
        m_SceneManager.SetActiveScene(new PlayScene());
    }

    SDL_Event f_Event;
    Timer f_DeltaTimer;
    float f_DeltaTime = 1.0f / 60.0f;

    while (m_Running)
    {
        // Event related work
        {
            while (SDL_PollEvent(&f_Event))
            {
                ProcessEvents(f_Event);
            }
        }

        // Update related work
        {
            ProcessUpdate(f_DeltaTime);
        }

        // Render related work
        {
            // Set clear color
            SDL_SetRenderDrawColor(m_Renderer, 64, 64, 64, 255);

            // Clear screen
            SDL_RenderClear(m_Renderer);

            // Renders everything
            ProcessRender(m_Renderer);

            // Update screen
            SDL_RenderPresent(m_Renderer);
        }

        // Resets the delta time
        {
            f_DeltaTime = static_cast<float>(f_DeltaTimer.GetTicks()) / 1000.0f;
            f_DeltaTimer.Start();
        }
    }
}

WindowSettings& Game::GetSettings()
{
    return m_Settings;
}

SceneManager& Game::GetSceneManager()
{
    return m_SceneManager;
}

void Game::ProcessEvents(SDL_Event& p_Event)
{
    if (p_Event.type == SDL_WINDOWEVENT)
    {
        if (p_Event.window.type == SDL_WINDOWEVENT_RESIZED)
        {
            SDL_Log("Window %d resized to %dx%d", p_Event.window.windowID, p_Event.window.data1, p_Event.window.data2);
        }
    }

    m_SceneManager.GetActiveScene().GetEntityManager().ProcessEvents(p_Event);
    m_SceneManager.GetActiveScene().ProcessEvents(p_Event);

    if (p_Event.type == SDL_QUIT) {
        m_Running = false;
    }
}

void Game::ProcessUpdate(float p_DeltaTime)
{
    m_SceneManager.GetActiveScene().GetEntityManager().ProcessUpdate(p_DeltaTime);
    m_SceneManager.GetActiveScene().ProcessUpdate(p_DeltaTime);
}

void Game::ProcessRender(SDL_Renderer* p_Renderer)
{
    m_SceneManager.GetActiveScene().GetEntityManager().ProcessRender(p_Renderer);
    m_SceneManager.GetActiveScene().ProcessRender(p_Renderer);
}