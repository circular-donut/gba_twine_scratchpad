#include "bn_core.h"

#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_colors.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_string_view.h"

#include "fixed_32x64_sprite_font.h"

#include "bn_sprite_items_variable_8x16_font_red.h"
#include "bn_sprite_items_variable_8x16_font_blue.h"
#include "bn_sprite_items_variable_8x16_font_yellow.h"

#include "common_fixed_8x8_sprite_font.h"
#include "common_fixed_8x16_sprite_font.h"
#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include <array>
#include <string>

namespace firstStoryStrings
{

    bn::string_view introScreen[3] = {"Hello welcome to my", "twisted", "game"};
    bn::string_view choiceScreen[3] = {"Press A if you're not twisted", "Press B if you are twisted", ""};
    bn::string_view responsePositive[3] = {"HAHAHAHA", "I like you", ""};
    bn::string_view responseNegative[3] = {"Ok sorry", "For freaking you out", ""};

    int sceneCount = 4;
    int currentSceneIndex = 0;

    bn::string_view *scenes[4] = {introScreen, choiceScreen, responsePositive, responseNegative};

    bn::string_view *currentScenePointer = scenes[0];

    bn::string_view *getCurrentScene()
    {
        return currentScenePointer;
    }

    void advanceCurrentScene()
    {
        if (currentSceneIndex > sceneCount)
        {
            currentSceneIndex = 0;
        }
        else
        {
            currentSceneIndex++;
        }
        currentScenePointer = scenes[currentSceneIndex];
    }
}

namespace textGen
{
    constexpr bn::fixed text_y_inc = 14;
    constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;
    bool shouldAdvanceScene = false;
    bn::string_view *sceneArray = firstStoryStrings::getCurrentScene();

    void advanceScene()
    {
        firstStoryStrings::advanceCurrentScene();
    }

    void generateTextScene()
    {
        bn::sprite_text_generator small_fixed_text_generator(common::fixed_8x8_sprite_font);
        bn::vector<bn::sprite_ptr, 32> text_sprites;

        bn::fixed text_y = -text_y_inc - (text_y_inc / 2);
        for (int i = 0; i < 3; i++)
        {
            bn::string_view currentText = sceneArray[i];

            small_fixed_text_generator.generate(-90, text_y, currentText, text_sprites);
            text_y += text_y_inc;
        }

        while (!bn::keypad::start_pressed())
        {
            bn::core::update();
        }

        advanceScene();

        sceneArray = firstStoryStrings::getCurrentScene();
    }
}

int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));
    while (true)
    {
        textGen::generateTextScene();
        bn::core::update();
    }
}