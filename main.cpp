#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

// Constants
const int WIDTH = 800;
const int HEIGHT = 600;
const int BAR_WIDTH = 10;
const int NUM_BARS = WIDTH / BAR_WIDTH;
const int DELAY = 100;

// Enumeration for the different states of the program
enum class State
{
    Menu,
    Sorting
};

// Generate a random list of integers
std::vector<int> generateRandomList(int size)
{
    std::vector<int> list(size);
    for (int i = 0; i < size; i++)
    {
        list[i] = rand() % HEIGHT;
    }
    return list;
}

// Merge two sorted lists into one sorted list
std::vector<int> merge(std::vector<int> left, std::vector<int> right, std::vector<sf::RectangleShape>& bars, sf::RenderWindow& window, int delay)
{
    std::vector<int> result;
    int i = 0; // Index of the current bar
    while (left.size() > 0 || right.size() > 0)
    {
        if (left.size() > 0 && right.size() > 0)
        {
            if (left.front() <= right.front())
            {
                result.push_back(left.front());
                left.erase(left.begin());
            }
            else
            {
                result.push_back(right.front());
                right.erase(right.begin());
            }
        }
        else if (left.size() > 0)
        {
            result.insert(result.end(), left.begin(), left.end());
            break;
        }
        else if (right.size() > 0)
        {
            result.insert(result.end(), right.begin(), right.end());
            break;
        }

        // Update the position of the current bar
        bars[i].setSize(sf::Vector2f(BAR_WIDTH, result[i]));
        bars[i].setPosition(i * BAR_WIDTH, HEIGHT - result[i]);

        // Display the updated bar
        window.clear(sf::Color::White);
        for (int j = 0; j < bars.size(); j++)
        {
            window.draw(bars[j]);
        }
        window.display();

        // Delay
        sf::sleep(sf::milliseconds(delay));

        i++; // Move to the next bar
    }

    return result;
}

std::vector<int> mergeSort(std::vector<int> list, std::vector<sf::RectangleShape>& bars, sf::RenderWindow& window, int delay)
{
    if (list.size() <= 1)
    {
        return list;
    }

    std::vector<int> left, right, result;
    int middle = list.size() / 2;

    for (int i = 0; i < middle; i++)
    {
        left.push_back(list[i]);
    }

    for (int i = middle; i < list.size(); i++)
    {
        right.push_back(list[i]);
    }

    left = mergeSort(left, bars, window, delay);
    right = mergeSort(right, bars, window, delay);
    result = merge(left, right, bars, window, delay);

    // Update the positions of the bars
    for (int i = 0; i < result.size(); i++)
    {
        // Update the position of the bar
        bars[i].setSize(sf::Vector2f(BAR_WIDTH, result[i]));
        bars[i].setPosition(i * BAR_WIDTH, HEIGHT - result[i]);
    }

    return result;
}

int main()
{
    // Initialize the random number generator
    srand(time(0));

    // Load the font
    sf::Font font;
    if (!font.loadFromFile("pkfont.ttf"))
    {
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }

    // Initialize the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Merge Sort Visualizer");

    // Initialize the list of bars
    std::vector<int> list = generateRandomList(NUM_BARS);
    std::vector<sf::RectangleShape> bars(NUM_BARS);
    for (int i = 0; i < NUM_BARS; i++)
    {
        bars[i].setSize(sf::Vector2f(BAR_WIDTH, list[i]));
        bars[i].setPosition(i * BAR_WIDTH, HEIGHT - list[i]);
        bars[i].setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
    }

    // Initialize the state of the program
    State state = State::Menu;

    // Main loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Handle events
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                // Handle key presses
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Enter && state == State::Menu)
                {
                    // Start sorting when the user presses Enter on the menu screen
                    state = State::Sorting;
                }
            }
        }

        // Update the display based on the current state of the program
        if (state == State::Menu)
        {
            // Display the menu screen
            window.clear(sf::Color::White);
            sf::Text text("Press Enter to start sorting", font, 24);
            text.setOutlineColor(sf::Color::Black);
            text.setOutlineThickness(5.f);
            text.setPosition(WIDTH / 2 - text.getGlobalBounds().width / 2, HEIGHT / 2 - text.getGlobalBounds().height / 2);
            window.draw(text);
            window.display();
        }
        else if (state == State::Sorting)
        {
            // Sort the list and update the display
            list = mergeSort(list, bars, window, DELAY);

            // Return to the menu screen once the sorting is complete
            state = State::Menu;
        }
    }

    return 0;
}
