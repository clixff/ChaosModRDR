#pragma once

#include <stdint.h>
#include <vector>
#include <string>

enum class EModSubmenu : uint8_t
{
	Main, Effects, Settings
};

class ModMenuButton
{
public:
	ModMenuButton();
	
	~ModMenuButton();
	
	std::string text = "";
	
	virtual void Draw(uint8_t index);
	
	virtual void OnKeyPressed(uint8_t key);
	
	virtual void OnSelected();
};

class ModMenu
{
public:
	ModMenu();
	
	~ModMenu();
	
	bool IsVisible();
	
	void ToggleVisibility();
	
	void Draw();
	
	void OnKeyPressed(uint8_t key);

private:
	bool bMenuEnabled = false;
	EModSubmenu submenu;
	
	std::vector <ModMenuButton> buttons;
	
	uint32_t buttonSelectedID = 0;
	
	EModSubmenu prevSubMenu;
	
	std::string menuTitle;
	
	uint32_t maxButtons = 9;
	
	void OpenSubmenu(EModSubmenu menu);
};

