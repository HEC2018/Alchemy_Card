#include "ascii_graphics.h"
#include <iostream>
#include <string>
using namespace std;

// in order to create a minion,
// minion without ability: f(name,cost,attack,defence)
// minion with triggered ability: f(name,cost,attack,defence,trigger description)
// minion with activated ability: f(name,cost,attack,defence, ability cost, ability description)

// inorder to create a ritual:
// display_ritual(name,cost,ritual_cost, ritual_description,ritual_charge)

// spell:
// display_spell(name,cost,description)

// player:
// display_player_card(player_num, name,life, magic)



string f(){
	vector<card_template_t>player;
	player.push_back(display_minion_no_ability("Air Elemental", 0, 12345678, 1234567890));
	player.push_back(display_minion_no_ability("Air Elemental", 0, 1, 1));
	player.push_back(display_minion_no_ability("Air Elemental", 0, 1, 1));
	player.push_back(display_minion_triggered_ability("Yunkai",1,1,1,"This is huge"));
	

	string s = "";

	// Draw the top line
	s += EXTERNAL_BORDER_CHAR_TOP_LEFT;
	for (int i = 0; i < 33*5; i++){
		s += EXTERNAL_BORDER_CHAR_LEFT_RIGHT;
	}
	s += EXTERNAL_BORDER_CHAR_TOP_RIGHT+"\n";

	
	// Draw the first row, including rituals, player, and graveyard.

	for (int i = 0 ; i < 11; i++){
		s += EXTERNAL_BORDER_CHAR_UP_DOWN;
		s += display_ritual("Aura",1,2,"this is a ritual", 3)[i];
		s += CARD_TEMPLATE_EMPTY[i];
		s += display_player_card(1,"yunkai",10,10)[i];
		s += CARD_TEMPLATE_EMPTY[i];
		s += display_minion_no_ability("Air",0,0,1)[i];
		s += EXTERNAL_BORDER_CHAR_UP_DOWN;
		s += "\n";
	}


	for (int i = 0; i < 11; i++){
		s += EXTERNAL_BORDER_CHAR_UP_DOWN;
		for (int j = 0; j < 5; j++){
			s += (player.size()>j ? player[j][i] : CARD_TEMPLATE_BORDER[i]);
		}
		s += EXTERNAL_BORDER_CHAR_UP_DOWN;
		s += "\n";
	}
	
	for (int i = 0 ; i < 10; ++i){
		s += CENTRE_GRAPHIC[i];
		s += "\n";
	}
	for (int i = 0; i < 11; i++){
		s += EXTERNAL_BORDER_CHAR_UP_DOWN;
		for (int j = 0; j < 5; j++){
			s += (player.size()>j ? player[j][i] : CARD_TEMPLATE_BORDER[i]);
		}
		s += EXTERNAL_BORDER_CHAR_UP_DOWN;
		s += "\n";
	}
	for (int i = 0 ; i < 11; i++){
		s += EXTERNAL_BORDER_CHAR_UP_DOWN;
		s += display_ritual("Aura",1,2,"this is a ritual", 3)[i];
		s += CARD_TEMPLATE_EMPTY[i];
		s += display_player_card(0,"yunkai",10,10)[i];
		s += CARD_TEMPLATE_EMPTY[i];
		s += display_minion_no_ability("Air",0,0,1)[i];
		s += EXTERNAL_BORDER_CHAR_UP_DOWN;
		s += "\n";
	}

	s += EXTERNAL_BORDER_CHAR_BOTTOM_LEFT;
	for (int i = 0; i < 33*5; i++){
		s += EXTERNAL_BORDER_CHAR_LEFT_RIGHT;
	}
	s += EXTERNAL_BORDER_CHAR_BOTTOM_RIGHT+"\n";	
	return s;
}

int main(){
	cout << f();
	return 0; 
}
