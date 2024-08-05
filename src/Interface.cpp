#include <pybind11/embed.h> // everything needed for embedding
#include <pybind11/eval.h>
namespace py = pybind11;

int main() {
    py::scoped_interpreter guard{}; // start the interpreter and keep it alive

    py::object scope = py::module_::import("__main__").attr("__dict__");
    py::eval_file("../Apple_API.py", scope);
}
