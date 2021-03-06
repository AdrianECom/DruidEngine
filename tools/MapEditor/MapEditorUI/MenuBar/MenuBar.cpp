#include "MenuBar.hpp"

#include "Core/TimeUtils.hpp"

#include "Log/Log.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Matrix4.hpp"
#include "Input/Input.hpp"
#include "Containers/List.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/Array.hpp"

#include "Graphics/Mesh.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Graphics/Animation/Animation.hpp"

#include "Physics/PhysicsEngine.hpp"
#include "Physics/Collider.hpp"
#include "Physics/RigidBody.hpp"

#include "Scene/Scene.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/RenderContext.hpp"

#include "File/File.hpp"
#include "UI/UI.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"
#include "UI/UITextEditable.hpp"

#include "Config/EngineConfig.hpp"

#include "Core/TimerManager.hpp"

#include "Scene/GameObjectBuilder.hpp"
#include "MapEditor.hpp"
#include "../MapEditorUI.hpp"
#include "EditorEvents/EditorEvents.hpp"
#include "Editor/EditorBuilder.hpp"

namespace DE {

MenuBar::MenuBar(){

}

MenuBar::~MenuBar(){

}

void MenuBar::init(MapEditor *mapEditor) {
	mMapEditor = mapEditor;
	createMenuBar();
}

void MenuBar::createMapElement(StringRef materialPath) {
	mMapEditor->mMapEditorUI.mBrush.setMapElementData(&mMapElementData_ActionPoint);
	mMapEditor->mMapEditorUI.mBrush.clear();
	Material* material = MaterialManager::getInstance()->loadMaterial(materialPath);
	mMapEditor->mMapEditorUI.mBrush.setMaterial(material);
	f32 gridTileSize = mMapEditor->mGrid.getTileSize();
	mMapEditor->mMapEditorUI.mBrush.setDrawTileSize(Vector2(gridTileSize, gridTileSize));
	mMapEditor->mMapEditorUI.mBrush.setIsPaintMode(true);
} 

void MenuBar::createMenuBar() {

	Scene* scene = mMapEditor->getGameObject()->getScene();

	f32 baseY = 0.95f;

	EditorBuilder::getInstance()->createPanel(Vector2(-1 * RenderContext::getAspectRatio(),1), Vector2(2.0f * RenderContext::getAspectRatio(), 0.1f));

	UI::getInstance()->getBuilder()->
		setLayout(UILayout::HORIZONTAL)->
		setPosition(Vector2(-1 * RenderContext::getAspectRatio(), 1))->
		setTextSize(UI::getInstance()->getDefaultFontSize()*1.5f)->
		setAdjustSizeToText(true)->
		setLayer(mMapEditor->mMapEditorUI.mUILayer);

	EditorBuilder::getInstance()->createDropdown("File")->
		addOption("New", [&](UIElement* uiElement) {})->
		addOption("Save", [&, scene = scene](UIElement* uiElement) {
			scene->saveScene(scene->getPath());
		})->
		addOption("Load", [&](UIElement* uiElement) {})->
		addOption("Close", [&](UIElement* uiElement) {});

	EditorBuilder::getInstance()->createDropdown("View")->
		addOption(StringsUI::getInstance()->Collider, [&](UIElement* uiElement) {
			PhysicsEngine::getInstance()->setDebugColliders(!PhysicsEngine::getInstance()->getDebugColliders());
		})->
		addOption(StringsUI::getInstance()->Grid, [&, scene = scene](UIElement* uiElement) {
			mMapEditor->mMapEditorUI.toggleGrid();
		})->
		addOption(StringsUI::getInstance()->Atlas, [&, scene = scene](UIElement* uiElement) {
			mMapEditor->mMapEditorUI.toggleAtlas();
		});

	EditorBuilder::getInstance()->createDropdown("Create")->
		addOption("Player Start", [&,this](UIElement* uiElement) {
			createMapElement("resources/editor-icons/PlayerStart.png");
		})->
		addOption("Spawn Point", [&](UIElement* uiElement) {
			createMapElement("resources/editor-icons/SpawnPoint.png");
		})->
		addOption("Event Point", [&](UIElement* uiElement) {
			createMapElement("resources/editor-icons/EventPoint.png");
		})->
		addOption("Way Point", [&](UIElement* uiElement) {
			createMapElement("resources/editor-icons/WayPoint.png");
		})->
		addOption("Trigger Area", [&](UIElement* uiElement) {
			// TODO: To be implemented.
		})->
		addOption("Event Area", [&](UIElement* uiElement) {
			// TODO: To be implemented.
		})->
		addOption("Death Area", [&](UIElement* uiElement) {
			// TODO: To be implemented.
		});

	EditorBuilder::getInstance()->createDropdown("Brush")->
		addOption("Paint Mode", [&](UIElement* uiElement) {mMapEditor->mMapEditorUI.mBrush.setIsPaintMode(true);})->
		addOption("Select Mode", [&](UIElement* uiElement) {mMapEditor->mMapEditorUI.mBrush.setIsPaintMode(false);});

	EditorBuilder::getInstance()->createDropdown("Sprites")->
		addOption("Create", [&](UIElement* uiElement) { mMapEditor->mMapEditorUI.createSpriteFromBrush(); })->
		addOption("Add frame", [&](UIElement* uiElement) {});

	EditorBuilder::getInstance()->createButton(StringsUI::getInstance()->Play,[&](UIElement* uiElement) {
		if (mMapEditor->mPlayer) {
			mMapEditor->destroyPlayer();
		} else {
			mMapEditor->createPlayer();
		}

		mMapEditor->switchCameraControl();
	});
}

}
