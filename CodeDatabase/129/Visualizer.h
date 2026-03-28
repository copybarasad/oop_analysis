template<typename GraphicType>
class Visualizer{
private:
    GraphicType graphic_man;

public:
    void draw(Manager manager){
        graphic_man.display(manager);
        GField gfield = manager.sync();
        graphic_man.draw(gfield);
    }
};
