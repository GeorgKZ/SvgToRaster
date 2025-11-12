import nska_deserialize as nd

input_path = './Original.xml'

with open(input_path, 'rb') as f:
    try:
        deserialized_plist = nd.deserialize_plist(f, full_recurse_convert_nska=True, format=dict)
        print(deserialized_plist)
    except (nd.DeserializeError, 
            nd.biplist.NotBinaryPlistException, 
            nd.biplist.InvalidPlistException,
            nd.plistlib.InvalidFileException,
            nd.ccl_bplist.BplistError, 
            ValueError, 
            TypeError, OSError, OverflowError) as ex:
        # These are all possible errors from libraries imported

        print('Had exception: ' + str(ex))
        deserialized_plist = None

    if deserialized_plist:
        output_path_plist = input_path + '_deserialized.plist'
        output_path_json  = input_path + '_deserialized.json'

        nd.write_plist_to_json_file(deserialized_plist, output_path_json)
        nd.write_plist_to_file(deserialized_plist, output_path_plist)
