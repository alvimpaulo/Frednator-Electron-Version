{
    "targets": [{
        "target_name": "cvMatUtil",
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
        'cflags': [
            '-std=c++11', "<!(pkg-config opencv --cflags)",
        ],
        "cflags_cc!": ["-fno-exceptions"],
        'libraries': ["<!(pkg-config opencv --libs)"],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': ['NAPI_CPP_EXCEPTIONS'],
        "sources": ["./src/cpp/cvMatUtil.cc"],
    }, {
        "target_name": "videoUtil",
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
        'cflags': [
            '-std=c++11', "<!(pkg-config opencv --cflags)",
        ],
        "cflags_cc!": ["-fno-exceptions"],
        'libraries': ["<!(pkg-config opencv --libs)"],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': ['NAPI_CPP_EXCEPTIONS'],
        "sources": ["./src/cpp/videoUtil.cc"],
    }]
}
