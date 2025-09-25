#include <TBWG/essentials.h>
#include <TBWG/tool.h>
#include <TBWG/eventer.h>


void defaultToolUse(struct Tool* tool, struct World* w, struct Character* c, 
    struct EventerRequiredInformations eri)
{

}


struct Tool* getDefaultTool()
{
    struct Tool* tool = tbwgmalloc(sizeof(struct Tool));

    tool->toolCode;
    tool->sharpness;
    tool->mass = 0.5f;
    tool->required_informations = 0;
    tool->use = defaultToolUse;
}