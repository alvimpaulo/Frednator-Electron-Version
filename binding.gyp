{
    "targets": [{
        "target_name": "module",
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
        'cflags': [
            '-std=c++11', "<!(pkg-config opencv --cflags)", "-fno-exceptions",
        ],
         "cflags_cc!": [ "-fno-exceptions" ],
        'libraries': [ "<!(pkg-config opencv --libs)"],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
        "sources": [ "./src/cpp/module.cc"],
    }]
}