#include "ui/app.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "api/threads_api.hpp"
#include "api/posts_api.hpp"

using namespace ftxui;

enum class View {
    MainMenu,
    ViewThreads,
    CreateThread,
    ReadThread
};

void RunApp() {
    ScreenInteractive screen = ScreenInteractive::TerminalOutput();
    View current_view = View::MainMenu;

    std::vector<Thread> threads;
    std::vector<Post> posts;

    // ---------- MAIN MENU ----------
    std::vector<std::string> menu_entries = {
        "View Threads",
        "Create Thread",
        "Exit"
    };
    int menu_selected = 0;
    Component menu = Menu(&menu_entries, &menu_selected);

    menu |= CatchEvent([&](Event e) {
        if (e == Event::Return) {
            if (menu_selected == 0)
            {
                threads = get_threads();
                current_view = View::ViewThreads;
            }
            if (menu_selected == 1) 
            {
                current_view = View::CreateThread;
            }
            if (menu_selected == 2) {
                screen.Exit();
            }
            return true;
        }
        return false;
        });

    // Root Renderer
    Component app = Renderer(menu, [&] 
    {
        switch (current_view) {
        case View::MainMenu:
            return vbox({
                text("Forum [ Frodo ]") | bold | center,
                separator(),
                menu->Render(),
                }) | border;

        case View::ViewThreads:
            return vbox({
                text("Threads") | bold,
                separator(),
                text("Threads loaded: " + std::to_string(threads.size())),
                separator(),
                text("Press ESC to go back"),
                }) | border;

        case View::CreateThread:
            return vbox({
                text("Create Thread") | bold,
                separator(),
                text("TODO: Thread creation UI"),
                separator(),
                text("Press ESC to go back"),
                }) | border;

        default:
            return text("Unknown view");
        }
    });

    app |= CatchEvent([&](Event e) 
    {
        if (e == Event::Escape)
        {
            current_view = View::MainMenu;
            return true;
        }
        return false;
    });

    screen.Loop(app);
}
