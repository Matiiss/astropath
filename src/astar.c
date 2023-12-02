#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "./AS_astar.h"
#include "./AS_stack.h"
#include "./AS_dict.h"

typedef struct {
    PyObject_HEAD
    AS_ANode *node_arr;
    Py_ssize_t node_arr_length;
    AS_Dict *pos_dict;
} AstarObject;

static PyObject *astar_search(AstarObject *self, PyObject *args);
double euclidian_distance(AS_ANode *self, AS_ANode *target);
AS_Hash hash(PyObject *obj);
int eq_check(PyObject *obj1, PyObject *obj2);

static void
astar_dealloc(AstarObject *self) {
    for (size_t i = 0; i < self->pos_dict->nodes->length; ++i) {
        AS_DictNode *node = self->pos_dict->nodes->get_at(self->pos_dict->nodes, i);
        PyObject *key = node->key;
        Py_XDECREF(key);
    }
    self->pos_dict->free(self->pos_dict);
    free(self->pos_dict);

    for (Py_ssize_t i = 0; i < self->node_arr_length; ++i) {
        AS_ANode *node = &self->node_arr[i];
        free(node->data);
        free(node->neighbours);
    }

    // must free last because it contains a bunch of mallocs inside too (probably)
    free(self->node_arr);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static int
astar_init(AstarObject *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"nodes", NULL};
    PyObject *dict, *dict_keys, *dict_values;
    AS_ANode *node_arr;
    Py_ssize_t node_arr_length;
    AS_Dict *pos_dict;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &dict)) {
        return -1;
    }

    if (!PyDict_Check(dict)) {
        PyErr_SetString(PyExc_TypeError, "nodes must be a dictionary");
        return -1;
    }

    pos_dict = malloc(sizeof(AS_Dict));
    AS_DictInit(pos_dict, (AS_HashFunc)&hash, (AS_DictEqCheck)&eq_check);
    self->pos_dict = pos_dict;

    dict_keys = PyDict_Keys(dict);
    node_arr_length = PyList_Size(dict_keys);
    self->node_arr_length = node_arr_length;
    node_arr = malloc(node_arr_length * sizeof(AS_ANode));
    self->node_arr = node_arr;

    for (Py_ssize_t i = 0; i < node_arr_length; ++i) {
        AS_ANode node;
        PyObject *tpl = (void *)PyList_GetItem(dict_keys, i);
        Py_INCREF(tpl);

        double *vec = malloc(2 * sizeof(double));
        vec[0] = PyFloat_AsDouble(PyTuple_GET_ITEM(tpl, 0));
        vec[1] = PyFloat_AsDouble(PyTuple_GET_ITEM(tpl, 1));
        node.data = (void *)vec;
        node.data2 = (void *)tpl;
        node.distance_to = &euclidian_distance;
        node_arr[i] = node;

        pos_dict->set(pos_dict, tpl, (void *)&node_arr[i]);
    }

    Py_DECREF(dict_keys);

    // Python dicts are ordered, so keys and nodes are aligned
    dict_values = PyDict_Values(dict);

    for (Py_ssize_t i = 0; i < node_arr_length; ++i) {
        AS_ANode *node = &node_arr[i];
        PyObject *neighbours = PyList_GetItem(dict_values, i);

        node->neighbour_count = PySequence_Size(neighbours);
        node->neighbours = malloc(node->neighbour_count * sizeof(AS_ANode *));

        for (Py_ssize_t j = 0; j < (Py_ssize_t)node->neighbour_count; ++j) {
            PyObject *neighbour_pos = PySequence_GetItem(neighbours, j);
            node->neighbours[j] = pos_dict->get(pos_dict, neighbour_pos);
            Py_DECREF(neighbour_pos);
        }
    }

    Py_DECREF(dict_values);

    return 0;
}

// double euclidian_distance(AS_ANode *self, AS_ANode *target) {
//     double x1, y1, x2, y2;
//     PyObject *seq1 = (PyObject *)self->data;
//     PyObject *seq2 = (PyObject *)target->data;
//     x1 = PyFloat_AsDouble(PySequence_GetItem(seq1, 0));
//     y1 = PyFloat_AsDouble(PySequence_GetItem(seq1, 1));
//     x2 = PyFloat_AsDouble(PySequence_GetItem(seq2, 0));
//     y2 = PyFloat_AsDouble(PySequence_GetItem(seq2, 1));

//     return pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);
// }

double euclidian_distance(AS_ANode *self, AS_ANode *target) {
    double x1, y1, x2, y2;
    double *seq1 = self->data;
    double *seq2 = target->data;
    x1 = seq1[0];
    y1 = seq1[1];
    x2 = seq2[0];
    y2 = seq2[1];

    return pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);
}

AS_Hash hash(PyObject *obj) {
    return PyObject_Hash(obj);
}

int eq_check(PyObject *obj1, PyObject *obj2) {
    return PyObject_RichCompareBool(obj1, obj2, Py_EQ);
}

static PyObject *
astar_search(AstarObject *self, PyObject *args) {
    PyObject *start, *end, *ret_list;
    AS_ANode *node_start, *node_end;

    if (!PyArg_ParseTuple(args, "OO", &start, &end)) {
        return NULL;
    }

    node_start = self->pos_dict->get(self->pos_dict, start);
    node_end = self->pos_dict->get(self->pos_dict, end);

    if (AS_AStarSearch(node_start, node_end, &euclidian_distance, (AS_HashFunc)&hash, (AS_DictEqCheck)&eq_check) == 0) {
        AS_Stack stack;
        AS_AStarReconstructPath(node_end, &stack);
        Py_ssize_t length = stack.size;
        ret_list = PyList_New(length);
        for (Py_ssize_t i = 0; i < length; ++i) {
            PyObject *element = stack.pop(&stack);
            Py_INCREF(element);
            PyList_SetItem(ret_list, i, element);
        }

        stack.free(&stack);

        return ret_list;
    }

    Py_RETURN_NONE;
}

// static PyMethodDef astarMethods[] = {
//     {"search",  astar_search, METH_VARARGS,
//      NULL},
//     {NULL, NULL, 0, NULL}        /* Sentinel */
// };

static PyMethodDef astar_methods[] = {
    {"search", (PyCFunction)astar_search, METH_VARARGS, NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject AstarType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "astar.Astar",
    .tp_doc = NULL,
    .tp_basicsize = sizeof(AstarObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)astar_init,
    .tp_dealloc = (destructor)astar_dealloc,
    .tp_methods = astar_methods,
};

static struct PyModuleDef astarmodule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "astar",
    .m_doc = NULL,
    .m_size = -1

    // astarMethods
};

PyMODINIT_FUNC
PyInit_astar(void) {
    PyObject *m;
    if (PyType_Ready(&AstarType) < 0) {
        return NULL;
    }

    m = PyModule_Create(&astarmodule);
    if (m == NULL) {
        return NULL;
    }

    Py_INCREF(&AstarType);
    if (PyModule_AddObject(m, "Astar", (PyObject *)&AstarType) < 0) {
        Py_DECREF(&AstarType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
