def load_settings(settings_header_filepath):

    reading_settings = False
    reading_setters = False
    hash_map_variable_name = "name"
    settings = list()
    setters = dict()
    with open(settings_header_filepath) as file:
        line = file.readline()
        while line:
            if "// Hash map variable name" in line:
                words = line.split()
                for word in words:
                    if ";" in word:
                        hash_map_variable_name = word[:-1]
            
            if "// Not Directly Settable Variables" in line:
                reading_settings = False      

            
            if reading_settings:
                setting = line.strip()
                setting = setting[:-1]
                settings.append(setting)
            

            if "// Settable Class Variables" in line:
                reading_settings = True
   

            if "// Printers" in line:
                reading_setters = False
            
            if reading_setters:
                for setting in settings:
                    words = setting.split()
                    print(setting)
                    setting = words[1]

                    if setting in line and "Set" in line:
                        words = line.split()
                        setter = ""


                        for character in words[1]:
                            if character == "(":
                                break

                            setter = setter + character
                        
                        word = ""
                        for character in words[1]:
                            word = word + character

                            if character == "(":
                                word = ""

                            if word == "int" or word == "double" or word == "std::string":
                                setter = setter + f" {word}"
                                break
                        
                        for i in range(2, len(words)):
                            word = ""
                            for character in words[i]:
                                if character == " ":
                                    break
                                word = word + character
                                if word == "int" or word == "double" or word == "std::string":
                                    setter = setter + f" {word}"
                                    break
                        setters[setting] = setter
                                


            
            if "// Getters and Setters" in line:
                reading_setters = True


            line = file.readline()


    return settings, setters, hash_map_variable_name

def create_hash_map_function(settings, hash_map_variable_name):
    function_lines = ["void _CreateSettingsHashMap()"]
    function_lines.append("{")

    hash_counter = 1
    for settings_line in settings:
        words = settings_line.split()
        setting = words[1]

        function_line = "    " + hash_map_variable_name + ".insert ( std::pair<string, int>('" + setting + "', " + str(hash_counter) + ") );"
        function_lines.append(function_line)
        hash_counter += 1
    function_lines.append("}")

    print("----------------Hash map function")
    for line in function_lines:
        print(line)


def get_conversion_string(argument_type):
    if argument_type == "double":
        return "std::stod"
    elif argument_type == "int":
        return "std::stoi"
    elif argument_type == "std::string":
        return " "

def create_update_function(settings, setters):
    function_lines = ["void _UpdateSettingsFromFile(std::ifstream settings_file)"]
    function_lines.append("{")
    function_lines.append("    std::string line;")
    function_lines.append("    std::vector<std::string> words;")
    function_lines.append("    int setting_hash;")
    function_lines.append("    while (getline(settings_file, line))")
    function_lines.append("    {")
    function_lines.append("        words = split_sentence(line);")
    function_lines.append(" ")
    function_lines.append("        try")
    function_lines.append("        {")
    function_lines.append("            setting_hash = setting_hash_map.at(words[0]);")
    function_lines.append("        }")
    function_lines.append("        catch (const std::out_of_range)")
    function_lines.append("        {")
    function_lines.append("            setting_hash = 0;")
    function_lines.append("        }")
    function_lines.append(" ")
    function_lines.append("        switch (setting_hash)")
    function_lines.append("        {")
    function_lines.append("            case 0:")
    function_lines.append("                break;")

    for hash in range(len(settings)):
        setting = settings[hash]
        setter = setters[setting]
        if (setters == None):
            print(f"Warning: Setting {setting} missing setter.")
            continue
        function_lines.append("            case " + str(hash + 1) + ":")

        words = setter.split()
        setter_call = words[0] + "("
        words.pop(0)
        # Add arguments to setter call
        for i in range(len(words)):
            if i != 0:
                setter_call = setter_call + ", "
            conversion_string = get_conversion_string(words[i])
            setter_call = setter_call + f"{conversion_string}(words[{i + 1}])"


        setter_call = setter_call + ");"
        setter_call = "                " + setter_call
        function_lines.append(setter_call)
        function_lines.append("                break;")



    function_lines.append("        }")
    


    function_lines.append("}")

    print("----------------Update function")
    for line in function_lines:
        print(line)


def create_save_function(settings):
    print("----------------Save function")
    function_lines = ["void _SaveToFile()"]
    function_lines.append("{")
    function_lines.append("    std::ofstream settings_file;")
    function_lines.append("    settings_file.open('settings.ini');")
    function_lines.append(" ")

    for setting in settings:
        words = setting.split()
        setting = words[1]
        line = f"    settings_file << '{setting} =' << {setting} << std::endl;"
        function_lines.append(line)

    function_lines.append(" ")
    function_lines.append("    settings_file.close();")
    function_lines.append("}")

    for line in function_lines:
        print(line)

def main(settings_header_filepath = "../Settings.h"):
    settings, setters, hash_map_variable_name = load_settings(settings_header_filepath)


    create_hash_map_function(settings, hash_map_variable_name)
    create_update_function(settings, setters)
    create_save_function(settings)




main()