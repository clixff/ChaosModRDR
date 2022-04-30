#include "menu.h"
#include "main.h"
#include "natives.h"

ModMenu::ModMenu()
{
}

ModMenu::~ModMenu()
{
}

bool ModMenu::IsVisible()
{
	return this->bMenuEnabled;
}

void ModMenu::ToggleVisibility()
{
	bMenuEnabled = !bMenuEnabled;
	
	if (bMenuEnabled)
	{
		OpenSubmenu(EModSubmenu::Main);
		
		buttons = {};
		menuTitle = "Main menu";
		prevSubMenu = EModSubmenu::Main;
	}
	else
	{
		this->buttons = {};
	}
}

void ModMenu::Draw()
{
	/** Black background */
	GRAPHICS::DRAW_RECT(0.5f, 0.5f, 450.0f / 1920.0f, 450.0f / 1080.0f, 0, 0, 0, 205, 0, 0);
	
	/** Draw menu title */
	GRAPHICS::DRAW_RECT(0.5f, 0.5f - (245.0f / 1080.0f), 450.0f / 1920.0f, 40.0f / 1080.0f, 0, 0, 0, 255, 0, 0);
	
	/** Draw menu header */
	GRAPHICS::DRAW_RECT(0.5f, 0.5f - (307.5 / 1080.0f), 450.0f / 1920.0f, 125.0f / 1080.0f, 143, 6, 6, 255, 0, 0);
	
}

void ModMenu::OnKeyPressed(uint8_t key)
{
}

void ModMenu::OpenSubmenu(EModSubmenu menu)
{
	this->buttons.clear();
	switch (menu)
	{
		case EModSubmenu::Main:
			
			for (int32_t i = 0; i < 50; i++)
			{
				ModMenuButton button;
				button.text = "Button Test ";
				button.text += std::to_string(i + 1);
				
				buttons.push_back(button);
			}
			break;
		case EModSubmenu::Effects:
			break;
		case EModSubmenu::Settings:
			break;
		default:
			break;
	}
}

ModMenuButton::ModMenuButton()
{
}

ModMenuButton::~ModMenuButton()
{
}

void ModMenuButton::Draw(uint8_t index)
{
}

void ModMenuButton::OnKeyPressed(uint8_t key)
{
}


void ModMenuButton::OnSelected()
{
	
}
