import tds;

#include <SFML/Graphics.hpp>

// Any live cell with fewer than two live neighbors dies, as if by underpopulation.
// Any live cell with two or three live neighbors lives on to the next generation.
// Any live cell with more than three live neighbors dies, as if by overpopulation.
// Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

template <typename T>
void spawn_glider(T* map, uint32_t x, uint32_t y)
{
	// The glider is defined by five cells located at (x, y), (x+1, y), (x+2, y), (x+2, y+1) and (x+1, y+2)
	map[x    ][y] = true;
	map[x + 1][y] = true;
	map[x + 2][y] = true;
	map[x + 2][y + 1] = true;
	map[x + 1][y + 2] = true;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game of Life");

	const uint32_t grid_size = 128; // 128x128
	const uint32_t grid_size_bound = grid_size - 1; // For bound check

	bool map[grid_size][grid_size];
	bool map_copy[grid_size][grid_size];

	for (uint32_t y = 0; y < grid_size; y++)
	{
		for (uint32_t x = 0; x < grid_size; x++)
		{
			map[x][y] = false;

			//map[x][y] = rand() % 2;

			map_copy[x][y] = false;
		}
	}

	spawn_glider(map, 64, 64);
	spawn_glider(map, 34, 64);
	spawn_glider(map, 14, 54);
	//spawn_glider(map, 64, 61);

	uint32_t frame_count = 0;

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
				case sf::Event::KeyPressed:
					if (e.key.code == sf::Keyboard::Escape)
						window.close();
					break;
				case sf::Event::EventType::Closed:
					window.close();
					break;
			}
		}

		auto& map_read = frame_count % 2 == 0 ? map : map_copy;
		auto& map_write = frame_count % 2 == 1 ? map : map_copy;

		for (uint32_t y = 0; y < grid_size; y++)
		{
			for (uint32_t x = 0; x < grid_size; x++)
			{
				uint32_t cells_alive = 0;

				const uint32_t left_x = x > 0 ? x - 1 : grid_size_bound;
				const uint32_t right_x = x < grid_size_bound ? x + 1 : 0;
				const uint32_t up_y = y > 0 ? y - 1 : grid_size_bound;
				const uint32_t bottom_y = y < grid_size_bound ? y + 1 : 0;

				cells_alive += map_read[left_x][up_y] ? 1 : 0;
				cells_alive += map_read[left_x][y] ? 1 : 0;
				cells_alive += map_read[left_x][bottom_y] ? 1 : 0;

				cells_alive += map_read[right_x][up_y] ? 1 : 0;
				cells_alive += map_read[right_x][y] ? 1 : 0;
				cells_alive += map_read[right_x][bottom_y] ? 1 : 0;

				cells_alive += map_read[x][up_y] ? 1 : 0;
				cells_alive += map_read[x][bottom_y] ? 1 : 0;

				if (map_read[x][y])
				{
					if (cells_alive < 2)
						map_write[x][y] = false;

					if (cells_alive > 1)
						map_write[x][y] = true;

					if (cells_alive > 3)
						map_write[x][y] = false;
				}
				else
				{
					if (cells_alive == 3)
						map_write[x][y] = true;
					else
						// Because we ping pong, copy state in this case
						map_write[x][y] = false;
				}
			}	
		}

		window.clear();

		const uint32_t window_width = window.getSize().x;
		const uint32_t window_height = window.getSize().y;

		const float cell_size = (float)std::min(window_width, window_height) / grid_size;
		const uint32_t x_offset = static_cast<uint32_t>((float)window_width / 2.0f - grid_size * cell_size * 0.5f);
		const uint32_t y_offset = static_cast<uint32_t>((float)window_height / 2.0f - grid_size * cell_size * 0.5f);

		for (uint32_t y = 0; y < grid_size; y++)
		{
			for (uint32_t x = 0; x < grid_size; x++)
			{
				sf::RectangleShape r(sf::Vector2f(cell_size, cell_size));

				if (map_write[x][y])
				{
					r.setFillColor(sf::Color(255, 0, 0));
				}
				else
				{
					r.setFillColor(sf::Color(0, 0, 0));
				}

				r.setOutlineColor(sf::Color::Black);
				r.setOutlineThickness(-1);
				r.setPosition(x_offset + x * cell_size, y_offset + y * cell_size);
				window.draw(r);
			}
		}

		window.display();

		frame_count++;
	}
}