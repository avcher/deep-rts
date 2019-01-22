//
// Created by per on 06.06.18.
//

#include "PyGUI.h"

#include <pybind11/embed.h> // everything needed for embedding
#include "../../../python/bindings/Config.cpp"
#include "../../../python/bindings/Player.cpp"
#include "../../../python/bindings/Unit.cpp"
#include "../../../python/bindings/Tile.cpp"
#include "../../../python/bindings/Map.cpp"
#include "../../../python/bindings/Tilemap.cpp"
#include "../../../python/bindings/Game.cpp"
#include "../../../python/bindings/Constants.cpp"
#include "../Game.h"

PYBIND11_EMBEDDED_MODULE(pyDeepRTS, m) {
    init_Config(m);
    init_Player(m);
    init_Unit(m);
    init_Tile(m);
    init_Map(m);
    init_Tilemap(m);
    init_Game(m);
    init_Constants(m);
}

PyGUI::PyGUI(Game& game): game(game) {
    pybind11::initialize_interpreter();

    this->init_dependencies();
    this->init_argv();
    this->init_cython();
    this->init_gui();
}


PyGUI::~PyGUI() {
    //pybind11::finalize_interpreter();
}

void PyGUI::init_dependencies(){
    pybind11::module subprocess = pybind11::module::import("subprocess");
    pybind11::module sys = pybind11::module::import("sys");
    auto args = pybind11::list();
    args.append(sys.attr("executable"));
    args.append("-m");
    args.append("pip");
    args.append("install");
    args.append("numpy");
    args.append("pygame");
    args.append("cython");
    args.append("opencv-python");
    args.append("--user");
    subprocess.attr("check_call")(args);
}

void PyGUI::init_argv(){
    pybind11::module sys = pybind11::module::import("sys");
    auto args = pybind11::list();
    args.append("");
    sys.attr("argv") = args;
}

void PyGUI::init_cython(){
    pybind11::module pyximport = pybind11::module::import("pyximport");
    pyximport.attr("install")();
}

void PyGUI::init_gui(){
    pybind11::module GUI_MODULE = pybind11::module::import("gui");
    gui = GUI_MODULE.attr("GUI")(&this->game);
    this->gui_attr_render = gui.attr("render");
    this->gui_attr_view = gui.attr("view");

}

void PyGUI::capture() {
    gui.attr("capture")();
}

void PyGUI::view() {
    gui_attr_view();
}

void PyGUI::render() {
    gui_attr_render();
}

