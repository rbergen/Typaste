#include <vector>
#include <random>
#include <map>
#include <string>
#include <windows.h>

using namespace std;

class typo_map_type 
{
public:
    typedef map<WCHAR, vector<WCHAR>> typo_map_base; 

private:
    mt19937 gen;
    typedef uniform_int_distribution<> int_dist;
    typedef int_dist::param_type int_dist_params;
    int_dist dist;
    typo_map_base inner_map;
    
public:
    typo_map_type(typo_map_base map) :
        gen(random_device()()),
        dist(),
        inner_map(map)
    {}

    WCHAR get_for(WCHAR c)
    {
        auto element = inner_map.find(c);

        if (element == inner_map.end())
            return c;
        
        auto candidates = element->second;
        return candidates[dist(gen, int_dist_params(0, candidates.size() - 1))];
    }
};

class sound_config
{
    mt19937 gen;
    typedef uniform_int_distribution<> int_dist;
    typedef int_dist::param_type int_dist_params;
    int_dist dist;
    typedef pair<wstring, LPBYTE> key_data_pair;

public:
    sound_config(mt19937 generator) :
        gen(generator),
        dist(),
        key_list()
    {
        clear();
    }

    struct key_data : key_data_pair
    {
        using key_data_pair::key_data_pair;

        wstring& file_name() 
        {
            return first;
        }

        wstring& file_name(LPCWSTR value) 
        {
            first = value;
            return first;
        }

        wstring& file_name(const wstring &value) 
        {
            first = value;
            return first;
        }

        LPBYTE sound()
        {
            return second;
        }

        LPBYTE sound(LPBYTE value)
        {
            second = value;
            return second;
        }
    };

    void clear() 
    {
        enter = NULL;
        space = NULL;
        modifier_down = NULL;
        modifier_up = NULL;
        key.file_name(L"");
        key.sound(NULL);
        key_list.clear();
        random_key = true;
    }

    LPBYTE enter;
    LPBYTE space;
    LPBYTE modifier_down;
    LPBYTE modifier_up;
    key_data key;
    vector<key_data> key_list;
    bool random_key;

    LPBYTE get_key() 
    {
        return random_key ? key_list[dist(gen, int_dist_params(0, key_list.size() - 1))].sound() : key.sound();
    }

    LPBYTE enter_key() 
    {
        return enter != NULL ? enter : get_key();
    }

    LPBYTE space_key() 
    {
        return space != NULL ? space : get_key();
    }

    LPBYTE modifier_down_key() 
    {
        return modifier_down != NULL ? modifier_down : get_key();
    }

    LPBYTE modifier_up_key() 
    {
        return modifier_up != NULL ? modifier_up : get_key();
    }
};

class typaste_config
{
    mt19937 gen;

    typedef uniform_int_distribution<DWORD> dword_dist_type;
    typedef dword_dist_type::param_type dword_dist_params;
    dword_dist_type dword_dist;
    bernoulli_distribution bern_dist;
    typedef bernoulli_distribution::param_type bern_dist_params;

    double typo_chance_value;

public:

    typaste_config(DWORD defaultDelay, WORD defaultHotkey) :
        min_key_delay(defaultDelay),
        max_key_delay(defaultDelay),
        modifier_delay(defaultDelay),
        hotkey(defaultHotkey),
        gen(random_device()()),
        sound_config(gen),
        dword_dist(),
        bern_dist(),
        typo_chance_value(0.0),
        make_typos(false) 
    {}

    DWORD min_key_delay;
    DWORD max_key_delay;
    DWORD modifier_delay;
    sound_config sound_config;
    WORD hotkey;
    bool make_typos;

    DWORD key_delay() 
    {
        auto& delay_pair = minmax(min_key_delay, max_key_delay);
        return dword_dist(gen, dword_dist_params(delay_pair.first, delay_pair.second));
    }

    wstring typo_chance()
    {
        return to_wstring(typo_chance_value);
    }

    wstring typo_chance(wstring new_value)
    {
        try 
        {
            typo_chance_value = stod(new_value);
        }
        catch(...)
        {}

        return typo_chance();
    }

    bool make_typo()
    {
        if (!make_typos)
        {
            return false;
        }
        
        return bern_dist(gen, bern_dist_params(typo_chance_value));
    }
};

class paste_state
{
    typaste_config* config_ptr;
    wstring text_str;
    size_t text_index;
    
public:
    bool pasting;
    vector<HKL> keyboard_layouts;
    HKL original_keyboard_layout;
    BYTE last_flags;
    LPBYTE modifier_down_sound;
    LPBYTE modifier_up_sound;

    paste_state() 
    {
        clear();
    }

    void clear()
    {
        pasting = false;
        keyboard_layouts.clear();
        original_keyboard_layout = NULL;
        last_flags = 0;
    }

    void reset(typaste_config& config, LPCWSTR buffer) 
    {
        clear();
        config_ptr = &config;
        text(buffer);
        modifier_down_sound = config.sound_config.modifier_down_key();
        modifier_up_sound = config.sound_config.modifier_up_key();
    }

    typaste_config& config()
    {
        return *config_ptr;
    }

    void text(LPCWSTR buffer)
    {
        text_str = buffer;
        text_index = 0;
    }

    bool next_character() 
    {
        if (text_index >= text_str.length())
        {
            return false;
        }

        return ++text_index < text_str.length();
    }

    WCHAR character()
    {
        return text_index < text_str.length() ? text_str[text_index] : L'\0';
    }
};