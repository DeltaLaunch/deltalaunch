/*
* ΔLaunch
* Copyright (C) 2018  Reisyukaku
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "NewsMenu.hpp"

NewsMenu::NewsMenu(SDL_Rect pos) : Menu("News", pos){
	panX = 500;
    panY = 100;
}

NewsMenu::~NewsMenu() {
    //
}

void NewsMenu::AddStory(std::string title, std::string body, SDL_Texture *img) {
    u32 imgW = 256, imgH = 128, Y = 60;
    u32 space = 15+(imgH/2);
    u32 butCol = Graphics::GetDefaultSelCol();
    
    Panel *articles = new Panel(panX, panY);
    MenuElements.push_back(new Image(20, Y+(MenuElements.size()*space), imgW, imgH, img, 
    [&]()->Result{
        msgBox->Show("Test", "hello ", MSGBOX_OK);
        return 0;
    }));
    articles->AddString(10, 0, title);
    articles->AddString(10, 20, body);
    Panels.push_back(articles);
}

void NewsMenu::DrawButtons() {
    int ind = 0;
    for(auto button: MenuElements) {
        button->Draw(button->Pos, ind == menuOpt);
        ind++;
    }
}

/*
*   Trigger events
*/
void NewsMenu::Update() {
    if(!IsOpen()) return;
    Graphics::RenderTexture(Sprite, Pos);
    Graphics::DrawText(Fonts::FONT_LARGE, 30, 25, GetTitle());
    DrawButtons();
    Panels[menuOpt]->Update((bool)currLayer);
	
	if(Hid::Input & KEY_DUP) DecrementSelect();
	if(Hid::Input & KEY_DDOWN) IncrementSelect();
    if(Hid::Input & KEY_B) Back();
}