def load_settings(settings_path):
    pass

def create_hash_map_function(settings):
    pass

def create_update_function(settings, setters):
    pass

def create_save_function(settings):
    pass

def main(settings_path):
    settings, setters = load_settings(settings_path)
    create_hash_map_function(settings)
    create_update_function(settings, setters)
    create_save_function(settings)




main()