#include <vector>
#include <random>

class sound_config
{
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;

public:
    sound_config(std::mt19937 generator) :
        gen(generator),
        dist(),
        key_list()
    {
        clear();
    }

    void clear() 
    {
        enter = NULL;
        space = NULL;
        modifier_down = NULL;
        modifier_up = NULL;
        key.clear();
        key_list.clear();
        random_key = true;
    }

    LPCWSTR enter;
    LPCWSTR space;
    LPCWSTR modifier_down;
    LPCWSTR modifier_up;
    std::wstring key;
    std::vector<std::wstring> key_list;
    bool random_key;

    LPCWSTR get_key() 
    {
        std::uniform_int_distribution<>::param_type dist_params(0, key_list.size() - 1);
        return random_key ? key_list[dist(gen, dist_params)].c_str() : key.c_str();
    }

    LPCWSTR enter_key() 
    {
        return enter != NULL ? enter : get_key();
    }

    LPCWSTR space_key() 
    {
        return space != NULL ? space : get_key();
    }

    LPCWSTR modifier_down_key() 
    {
        return modifier_down != NULL ? modifier_down : get_key();
    }

    LPCWSTR modifier_up_key() 
    {
        return modifier_up != NULL ? modifier_up : get_key();
    }
};

class typaste_config
{
    std::mt19937 gen;
    std::uniform_int_distribution<DWORD> dist;

public:

    typaste_config(DWORD defaultDelay, WORD defaultHotkey) :
        min_key_delay(defaultDelay),
        max_key_delay(defaultDelay),
        modifier_delay(defaultDelay),
        hotkey(defaultHotkey),
        gen(std::random_device()()),
        sound_config(gen),
        dist() 
    {}

    DWORD min_key_delay;
    DWORD max_key_delay;
    DWORD modifier_delay;
    sound_config sound_config;
    WORD hotkey;

    DWORD key_delay() 
    {
        auto& delay_pair = std::minmax(min_key_delay, max_key_delay);
        return dist(gen, std::uniform_int_distribution<DWORD>::param_type(delay_pair.first, delay_pair.second));
    }
};