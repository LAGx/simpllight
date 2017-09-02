
#ifndef _INTERFACE_
#define _INTERFACE_

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "item.h"

class BaseClass;

/// one cell of interface. 
class CellInterface {
private:

	/// interface text, don`t use directly
	class Text {
	private:
		int id;///< id of this text
		sf::Text text;///< sfml text

		sf::Vector2f posRatio; ///< position of center of text ratio cell(0 by 100)
		int depthRender = 0; ///< depth render 

	public:
		/*!
		 @warning don`t use class directly. only across cellInterface methods
		 @param [in] text start text
		 @param [in] posRatio position of center of text ratio cell(0 by 100)
		 @param [in] scaleRatio scele factor  
		 @param [in] cell parent cell
		 */
		Text(int id, std::string text, sf::Vector2f posRatio, float scaleRatio, sf::Color textColor, sf::Font &font, CellInterface& cell);

		/// set position of text
		void setPosition(sf::Vector2f position);
		/// set scale factor 
		void setScale(float scale);
		///set new text
		void setNewText(std::string text);
		///set new color
		void setColor(sf::Color color);
		///set new size
		void setSize(float size);
		/// @return id of text
		int getId();
		/// change depth render by delta point
		void changeDepthRender(int delta);
		/// blit
		void blit();

	};

	///cell texture 
	class CellTexture {//gvnocod becouse of circular include
	public:
		sf::Texture texture;
		sf::Sprite g_body;

		void updateTextrPosition(sf::Vector2f newCord, float newAngle = 0);
		bool isVisible = true;

		int depthRender = 0;

		CellTexture(sf::Vector2f initCord, std::string texture);

		CellTexture();

		void blit();

	};

	sf::Color textColor; ///< default color of all text
	sf::Font font; ///< default font of all text
	vector<Text* > allText; ///< storage for all text
	
	void update();///< resolving small operations
	float animationCoef = 0; ///< for animation make 
	void animation(); ///< make animation 

	InterfaceItem* item = nullptr; ///< @warning have to be created outside

	/*!
	 @brief check if mouse on cell and set "isActive" true or false
	 */
	void phisicCollideDetecting();

	CellInterface::CellTexture *baseTextr;/// <background of texture
	CellInterface::CellTexture *frameTextr;///<frame of texture

	enum typeCell;
	typeCell type;///< type of cell 

	sf::Vector2f sizeCell;           ///< Size in pixel relative window. if it`s circle - first parameter is radius, second ignore
	sf::Vector2f positionRelWindow; ///<Position in pixel, @warning don`t have to update
	float speedChangeTransper = 1;  ///< from 0 (newer change) by 1 (in moment change)
	char deltaTransp = 0;          ///< in pixel. how more transperent it need to be in Quiet

	int depthRender = 0; ///< depth render of cell

	bool isVisible = true; ///< visibility of cell 

public:

	/// define style of cell
	struct StyleCell {
		/// generate texture
		sf::Color baseColor = { 0,0,0,255 };///<base color of background
		sf::Color frameColor = { 0,0,0 };///<base color of frane
		int borderSize = 1; ///< size of frame
		int shadow = 0; ///< size of shadow cell

		unsigned char deltaTransperActive = 0;///<how more transperent base, then 100%
		unsigned char deltaTransperQuiet = 0;///< transperent od background cell in usual state
		float speedChangeTransper = 0;///< speed of animation

		std::string textFont = "None";///< path to text font
		sf::Color textColor = {255,255,255};///<< start text color
	};

	///what type of sell. define form of cell
	enum typeCell {
		rect = 0,
		round
	};

	/*!
	 @warning position and scale reletive cell (in %)
	@param [in] mod can be "new"-new text(need almost parameters for create) and "del"-delete(need only id of deleting text)
	*/
	void textControl(std::string mod, int id, std::string text = "None", sf::Vector2f posRatio = { 0,0 }, float scaleRatio = 1);
	Text* getTextPtr(int id);

	void setItem(InterfaceItem* item);
	void removeItem();

	CellInterface(sf::Vector2f initRatio, sf::Vector2f sizeRatio, StyleCell& style, typeCell type, std::string id);

	std::string id = "None";

	bool isActive = false;

	void changeDepthRender(int depth);

	void blit();

	void setVisible(bool isVisible);

	~CellInterface();
};

/// layer of group of cells
class AssemblyLayerInterface {

	CellInterface::StyleCell getStyle(std::string styleId);
	void updateAllCellFromFile();

	std::vector<CellInterface*> allCell;
	std::string styleId = "None";//can be changed only once in constructor

public:

	AssemblyLayerInterface(std::string id, std::string mode, std::string styleId = "default"); //mod "new", "old"(can`t change style)

	CellInterface* getCellById(std::string id = "None");
	void createNewCell(sf::Vector2f initRatio, sf::Vector2f sizeRatio, CellInterface::typeCell type, std::string id, std::string styleId = "default");//for creating//can be using for update 
	void deleteCell(std::string id); //for creating
	void textControl(std::string cellId, std::string mod, int idText, std::string text = "None", sf::Vector2f posRatio = { 0,0 }, float scaleRatio = 1);


	std::string id = "None";

	std::string getActiveCell();

	void blit();

	~AssemblyLayerInterface();
};

///controller of several leyer
class UserInterfaceBox {
protected:
	std::vector<AssemblyLayerInterface*> fastAccessLayer;

	AssemblyLayerInterface* currInterfaceLayer = nullptr;
	
public:

	UserInterfaceBox();

	static std::string activeLayer;

	static std::string activeCell;

	void createLayer(std::string id, std::string styleId = "default"); //for creating

	void createStyle(std::string id, CellInterface::StyleCell& style); //for creating

	void downloadLayerById(std::string id);

	AssemblyLayerInterface* getCurrLayer();

	void blit();

	~UserInterfaceBox();
};
#endif