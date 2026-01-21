#include "ui/app.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "api/threads_api.hpp"
#include "api/posts_api.hpp"

using namespace ftxui;

void RunApp()
{
    ScreenInteractive screen = ScreenInteractive::TerminalOutput();
    auto spacer = text(" ");
    auto thin_spacer = text(" ");
    int tab_index = 0;

    /* --- Datos --- */
    std::vector<Post> posts;
    std::vector<Thread> threads;
    std::vector<std::string> thread_entries;
    Thread current_thread;

    std::string title;
    std::string myParagraph;
    std::string myText;

    /* --- Input --- */
    Component title_input = Input(&title, "title...");
    Component paragraph_input = Input(&myParagraph, "summary...");
    Component post_text_input = Input(&myText, "reply...");

    /* --- Buttons --- */
    Component create_thread_button = Button("create", [&]
        {
            if (!title.empty() && !myParagraph.empty())
            {
                create_thread(title, myParagraph);
                title.clear();
                myParagraph.clear();
                tab_index = 0;
            }
        });

    Component back_from_create_button = Button("back", [&]
        {
            title.clear();
            myParagraph.clear();
            tab_index = 0;
        });

    Component create_post_button = Button("post", [&]
        {
            if (!myText.empty())
            {
                bool check = create_post(current_thread, myText);
                myText.clear();
                posts = get_posts(current_thread);
                tab_index = 3;
            }
        });

    Component back_from_post_button = Button("back", [&]
        {
            myParagraph.clear();
            tab_index = 3;
        });

    /* --- Main menu --- */
    std::vector<std::string> main_menu_entries =
    {
        "view threads",
        "create thread",
        "exit"
    };

    int main_menu_selected = 0;
    Component main_menu = Menu(&main_menu_entries, &main_menu_selected);

    main_menu |= CatchEvent([&](Event e)
        {
            if (tab_index != 0)
                return false;

            if (e == Event::Return)
            {
                if (main_menu_selected == 0)
                {
                    threads = get_threads();
                    thread_entries.clear();
                    for (auto& t : threads)
                        thread_entries.push_back(t.title);
                    thread_entries.push_back("<< back");
                    tab_index = 1;
                }
                if (main_menu_selected == 1)
                {
                    tab_index = 2;
                }
                if (main_menu_selected == 2)
                {
                    screen.Exit();
                }
                return true;
            }
            return false;
        });

    /* --- Thread view --- */
    int thread_selected = 0;
    Component threads_menu = Menu(&thread_entries, &thread_selected);

    Component threads_view = Container::Vertical(
        {
            threads_menu
        });

    threads_view |= CatchEvent([&](Event e)
        {
            if (tab_index != 1)
                return false;

            if (e == Event::Return)
            {
                if (thread_entries.size() <= 1)
                {
                    tab_index = 0;
                }
                else
                {
                    if (thread_selected == thread_entries.size() - 1) // go back
                    {
                        tab_index = 0;
                    }
                    else // Pick a thread to read
                    {
                        current_thread = threads.at(thread_selected);
                        posts = get_posts(current_thread);
                        tab_index = 3;
                    }
                }
                return true;
            }
            return false;
        });

    /* --- Create thread view --- */
    Component create_thread_view = Container::Vertical(
        {
            title_input,
            paragraph_input,
            create_thread_button,
            back_from_create_button
        });

    /* --- Read posts from thread view --- */
    Component posts_view = Container::Vertical(
        {
            Button("reply", [&] { tab_index = 4; }),
            Button("back",  [&] { tab_index = 1; })
        });

    /* --- Create posts view --- */
    Component create_post_view = Container::Vertical(
        {
            post_text_input,
            create_post_button,
            back_from_post_button
        });

    /* -- Tabs --- */
    Component tabs = Container::Tab(
        {
            main_menu,
            threads_view,
            create_thread_view,
            posts_view,
            create_post_view
        }, &tab_index);

    /* --- Renderer for all views --- */
    Component app = Renderer(tabs, [&]
        {
            int max_width = screen.dimx() - 4;

            if (tab_index == 0) // Main menu
            {
                return vbox(
                    {
                        spacer,
                        text("Frodo-chan boards") | bold | center | color(Color::Aquamarine1Bis),
                        text("ver 0.1") | center | color(Color::GrayDark),
                        spacer,
                        separator(),
                        main_menu->Render() | center,
                        filler()
                    }) | frame | flex | border;
            }

            if (tab_index == 1) // Thread view
            {
                return vbox(
                    {
                        spacer,
                        text("threads") | bold | center | color(Color::Aquamarine1Bis),
                        separator(),
                        threads_menu->Render()
                            | size(WIDTH, LESS_THAN, max_width)
                            | frame
                            | vscroll_indicator
                            | flex,
                        filler()
                    }) | frame | flex | border;
            }

            if (tab_index == 2) // Thread creation view
            {
                return vbox(
                    {
                        spacer,
                        text("create thread") | bold | center | color(Color::Aquamarine1Bis),
                        separator(),
                        title_input->Render(),
                        spacer,
                        paragraph_input->Render()
                            | size(WIDTH, LESS_THAN, max_width)
                            | frame
                            | flex,
                        spacer,
                        hbox({
                            create_thread_button->Render(),
                            text("   "),
                            back_from_create_button->Render()
                        }) | center,
                        filler()
                    }) | frame | flex | border;
            }

            if (tab_index == 3) // Read a thread posts
            {
                std::string outputText = current_thread.title + " #" + std::to_string(current_thread.id);
                Elements shown_posts;

                for (auto& p : posts)
                {
                    shown_posts.push_back(
                        vbox({
                            text(">> post #" + std::to_string(p.id)) | color(Color::GrayDark),
                            paragraph(p.text)
                                | color(Color::White)
                                | size(WIDTH, LESS_THAN, max_width),
                            separator()
                            })
                    );
                }

                return vbox(
                    {
                        spacer,
                        text(outputText) | bold | center | blink | color(Color::Aquamarine1Bis),
                        text(current_thread.time) | center | color(Color::GrayDark),
                        spacer,
                        paragraph(current_thread.summary)
                            | center
                            | color(Color::White)
                            | size(WIDTH, LESS_THAN, max_width),
                        spacer,
                        vbox(shown_posts)
                            | size(WIDTH, LESS_THAN, max_width)
                            | frame
                            | vscroll_indicator
                            | flex,
                        spacer,
                        posts_view->Render() | center,
                        filler()
                    }) | frame | flex | border;
            }

            if (tab_index == 4) // Make a reply post
            {
                return vbox(
                    {
                        spacer,
                        text("reply") | bold | center | color(Color::Aquamarine1Bis),
                        separator(),
                        post_text_input->Render()
                            | size(WIDTH, LESS_THAN, max_width)
                            | frame
                            | flex,
                        spacer,
                        hbox({
                            create_post_button->Render(),
                            text("   "),
                            back_from_post_button->Render()
                        }) | center,
                        filler()
                    }) | frame | flex | border;
            }

            return text("");
        });

    /* --- Global events --- */
    app |= CatchEvent([&](Event e)
        {
            if (e == Event::Escape)
            {
                if (tab_index == 3)
                {
                    tab_index = 2;
                    myText.clear();
                    return true;
                }
                if (tab_index == 4)
                {
                    tab_index = 3;
                    myText.clear();
                    return true;
                }

                title.clear();
                myParagraph.clear();
                myText.clear();
                tab_index = 0;
                return true;
            }
            return false;
        });

    screen.Loop(app);
}
