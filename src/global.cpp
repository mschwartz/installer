/** 
 * @namespace global
 * 
 * ### Synopsis
 * 
 * Methods initially available in the global object/namespace.
 * 
 * ### Description
 * 
 * The core SilkJS executable provides these methods in the global scope.  Typically, they will be overloaded by other modules or JavaScript files being included or reuqired.
 * 
 * The global object also contains a global.builtin namespace, which consists of the other C++/native builtin bindings.
 * 
 * #### Note
 * The global object is roughly equivalent to the window object on the browser side.  You will note the method names below do no start with "global." - it's not required.
 */
#include "SilkJS.h"
#include <dlfcn.h>

using namespace v8;

Persistent<ObjectTemplate> globalObject;
Persistent<ObjectTemplate> builtinObject;
extern Persistent<Context> context;
extern char *readFile (const char *s);

/**
 * @function global.log
 * 
 * ### Synopsis
 * 
 * log(s);
 * 
 * Write a string to stdout.
 * 
 * The string is prefixed with the current process's pid.
 * 
 * @param {string} s - the string to write to stdout.
 */
static Handle<Value> Log (const Arguments& args) {
    HandleScope handle_scope;
    String::AsciiValue str(args[0]);
    printf("%ld %s\n", (unsigned long) getpid(), *str);
    return Undefined();
}

/**
 * @function global.print
 * 
 * ### Synopsis
 * 
 * print(s);
 * 
 * Write a string to stdout.
 * 
 * The string is NOT prefixed with the current process's pid, as is done by the log() function.
 * 
 * @param {string} s - the string to write to stdout.
 */
static Handle<Value> Print (const Arguments& args) {
    HandleScope handle_scope;
    String::AsciiValue str(args[0]);
    printf("%s", *str);
    return Undefined();
}

/**
 * @function global.println
 * 
 * ### Synopsis
 * 
 * println(s);
 * 
 * Write a string to stdout, adding a newline to the end.
 * 
 * The string is NOT prefixed with the current process's pid, as is done by the log() function.
 * 
 * @param {string} s - the string to write to stdout.
 */
static Handle<Value> Println (const Arguments& args) {
    HandleScope handle_scope;
    String::AsciiValue str(args[0]);
    printf("%s\n", *str);
    return Undefined();
}

struct SCRIPTNODE {
    SCRIPTNODE *next;
    const char *name;
    time_t mtime;
    Persistent<Script> script;
};

SCRIPTNODE *scriptCache = NULL;

/**
 * @function global.include
 * 
 * ### Synopsis
 * 
 * include(path [,path...]);
 * 
 * Load, compile, and run the specified JavaScript files in the SilkJS context of the current process.
 * 
 * @param {string} path - the path in the file system to a file to include.
 */
static Handle<Value> Include (const Arguments& args) {
    for (int i = 0; i < args.Length(); i++) {
        String::Utf8Value str(args[i]);
        char buf[strlen(*str) + 18 + 1];
        char *js_file = readFile(*str);
        if (!js_file) {
            strcpy(buf, *str);
            if (buf[0] != '/') {
                strcpy(buf, "/usr/local/silkjs/");
                strcat(buf, *str);
            }
            js_file = readFile(buf);
        }
        if (!js_file) {
            strcpy(buf, *str);
            if (buf[0] != '/') {
                strcpy(buf, "/usr/share/silkjs/");
                strcat(buf, *str);
            }
            js_file = readFile(buf);
        }
        if (!js_file) {
            return ThrowException(String::Concat(String::New("include file not found "), args[i]->ToString()));
        }
        Handle<String> source = String::New(js_file);
        delete [] js_file;
        ScriptOrigin origin(String::New(*str), Integer::New(0), Integer::New(0));
        Handle<Script>script = Script::New(source, &origin);
        script->Run();
    }
    return Undefined();
}

static Handle<Value> LoadLibrary(const Arguments& args) {
    String::Utf8Value filename(args[0]);
    int flag = RTLD_LAZY | RTLD_GLOBAL; // args[1]->IntegerValue();
    void *handle = dlopen(*filename, flag);
    if (!handle) {
        printf("1) %s\n", dlerror());
        return ThrowException(String::Concat(String::New("Shared library not found "), args[0]->ToString()));
    }
    typedef JSOBJ (*getExports_t)();
    getExports_t getExports = (getExports_t)dlsym(handle, "getExports");
    if (!getExports) {
        printf("2) %s\n", dlerror());
        return ThrowException(String::Concat(String::New("Shared library does not contain getExports function "), args[0]->ToString()));
    }

    return getExports();
}

// extern void init_buffer_object ();
extern void init_console_object ();
extern void init_process_object ();
extern void init_v8_object ();
extern void init_net_object ();
extern void init_fs_object ();
extern void init_curl_object ();

void init_global_object () {
    globalObject = Persistent<ObjectTemplate>::New(ObjectTemplate::New());
    builtinObject = Persistent<ObjectTemplate>::New(ObjectTemplate::New());

    init_console_object();
    init_process_object();
    init_net_object();
    init_fs_object();
    init_v8_object();
    init_curl_object();

    globalObject->Set(String::New("builtin"), builtinObject);
    globalObject->Set(String::New("log"), FunctionTemplate::New(Log));
    globalObject->Set(String::New("print"), FunctionTemplate::New(Print));
    globalObject->Set(String::New("println"), FunctionTemplate::New(Println));
    globalObject->Set(String::New("include"), FunctionTemplate::New(Include));
    globalObject->Set(String::New("loadDll"), FunctionTemplate::New(LoadLibrary));
}
