//version 1.5

#include <GXCommon/gx_obj_loader.h>
#include <GXCommon/gx_log.h>
#include <QFile>

namespace gx_common {

static int gx_objloader_Index = 0;
static char* gx_objloader_ObjSource = nullptr;
static unsigned int gx_objloader_FileSize = 0;
static char gx_objloader_T[ 30 ] = { 0 };

static unsigned int gx_objloader_TotalVertexes = 0;
static unsigned int gx_objloader_TotalNormals = 0;
static unsigned int gx_objloader_TotalUVs = 0;
static unsigned int gx_objloader_TotalTriangles = 0;

struct GXOBJIndex {
    int v;
    int vn;
    int vt;
};

struct GXOBJVertex {
    GXVec3 position;
    GXOBJVertex* next;
};

struct GXOBJNormals {
    GXVec3 normal;
    GXOBJNormals* next;
};

struct GXOBJUV_s {
    GXVec2 uv;
    GXOBJUV_s* next;
};

struct GXOBJTriangle {
    GXOBJIndex dat[3];
    GXOBJTriangle* next;
};

//-----------------------------------------------------------------------

void __stdcall skipComment()
{
    while (gx_objloader_ObjSource[gx_objloader_Index] != '\n'
           && (gx_objloader_Index < (int)gx_objloader_FileSize - 1)) {
        gx_objloader_Index++;
    }

    gx_objloader_Index++;
}

void __stdcall skipGaps()
{
    while (gx_objloader_ObjSource[gx_objloader_Index] == ' ' && (gx_objloader_Index < (int)gx_objloader_FileSize - 1)) {
        gx_objloader_Index++;
    }
}

void __stdcall solveNewLine()
{
    while (gx_objloader_ObjSource[gx_objloader_Index] != 'v'
           && gx_objloader_ObjSource[gx_objloader_Index] != 'f'
           && (gx_objloader_Index < (int)gx_objloader_FileSize - 1)) {
        switch (gx_objloader_ObjSource[gx_objloader_Index]) {
            case '#': {
                skipComment();
                break;
            }
            case '\n': {
                gx_objloader_Index++;
                break;
            }
            default: {
                skipComment();
                break;
            }
        }

        skipGaps();
    }
}

template <class Pointer>
void __stdcall deleteList(Pointer top)
{
    Pointer p = top;
    while (top != 0) {
        top = top->next;
        delete p;
        p = top;
    }
}

GXOBJVertex* __stdcall gxParseVertices()
{
    GXOBJVertex *vTop;
    GXOBJVertex *vTek;
    int i = 0;

    vTop = new GXOBJVertex;
    vTek = vTop;
    vTek->next = 0;

    while (true) {
        vTek->next = new GXOBJVertex;
        vTek = vTek->next;
        vTek->next = 0;

        gx_objloader_TotalVertexes++;

        skipGaps();
        i = 0;
        while (gx_objloader_ObjSource[gx_objloader_Index] != ' ') {
            gx_objloader_T[i] = gx_objloader_ObjSource[gx_objloader_Index];
            gx_objloader_Index++;
            i++;
        }

        gx_objloader_T[i] = '\0';
        vTek->position.x = (float)atof(gx_objloader_T);

        skipGaps();
        i = 0;
        while (gx_objloader_ObjSource[gx_objloader_Index] != ' ') {
            gx_objloader_T[i] = gx_objloader_ObjSource[gx_objloader_Index];
            gx_objloader_Index++;
            i++;
        }

        gx_objloader_T[i] = '\0';
        vTek->position.y = (float)atof(gx_objloader_T);

        skipGaps();
        i = 0;
        while (gx_objloader_ObjSource[gx_objloader_Index] != '\n'
               && gx_objloader_ObjSource[gx_objloader_Index] != ' ') {
            gx_objloader_T[i] = gx_objloader_ObjSource[gx_objloader_Index];
            gx_objloader_Index++; 
            i++;
        }

        gx_objloader_T[i] = '\0';
        vTek->position.z = (float)atof(gx_objloader_T);

        skipComment();
        skipGaps();
        solveNewLine();

        gx_objloader_Index++;
        if (gx_objloader_ObjSource[gx_objloader_Index] != ' ' ) break;
    }

    vTek = vTop; 
    vTop = vTop->next; 
    delete vTek;

    return vTop;
}


GXOBJNormals* __stdcall gxParseNormals()
{
    GXOBJNormals* nTop = new GXOBJNormals;
    GXOBJNormals* nTek = nTop;
    nTek->next = 0;
    int i = 0;

    gx_objloader_Index++;

    while (true) {
        skipGaps();

        nTek->next = new GXOBJNormals;
        nTek = nTek->next;
        nTek->next = 0;

        gx_objloader_TotalNormals++;

        i = 0;
        while (gx_objloader_ObjSource[gx_objloader_Index] != ' ') {
            gx_objloader_T[i] = gx_objloader_ObjSource[gx_objloader_Index];
            gx_objloader_Index++;
            i++;
        }

        gx_objloader_T[i] = '\0';
        nTek->normal.x = (float)atof(gx_objloader_T);

        skipGaps();
        i = 0;
        while (gx_objloader_ObjSource[ gx_objloader_Index ] != ' ') {
            gx_objloader_T[i] = gx_objloader_ObjSource[gx_objloader_Index];
            gx_objloader_Index++;
            i++;
        }

        gx_objloader_T[i] = '\0';
        nTek->normal.y = (float)atof(gx_objloader_T);

        skipGaps();
        i = 0;
        while (gx_objloader_ObjSource[gx_objloader_Index] != '\n'
               && gx_objloader_ObjSource[ gx_objloader_Index ] != ' ') {
            gx_objloader_T[i] = gx_objloader_ObjSource[gx_objloader_Index];
            gx_objloader_Index++;
            i++;
        }

        gx_objloader_T[i] = '\0';
        nTek->normal.z = (float)atof(gx_objloader_T);

        skipComment();
        skipGaps();
        solveNewLine();

        gx_objloader_Index++;
        if (gx_objloader_ObjSource[gx_objloader_Index] != 'n') break;
        gx_objloader_Index++;
    }

    nTek = nTop; 
    nTop = nTop->next; 
    delete nTek;

    return nTop;
}

GXOBJUV_s* __stdcall gxParseUVs()
{
    GXOBJUV_s* uvTop = new GXOBJUV_s;
    GXOBJUV_s* uvTek = uvTop;
    uvTek->next = 0;
    int i = 0;
    gx_objloader_Index++;

    while (true) {
        skipGaps();

        uvTek->next = new GXOBJUV_s;
        uvTek = uvTek->next;
        uvTek->next = 0;

        gx_objloader_TotalUVs++;

        i = 0;
        while (gx_objloader_ObjSource[gx_objloader_Index] != ' ') {
            gx_objloader_T[i] = gx_objloader_ObjSource[gx_objloader_Index];
            gx_objloader_Index++;
            i++;
        }

        gx_objloader_T[i] = '\0';
        uvTek->uv.u = (float)atof(gx_objloader_T);

        skipGaps();
        i = 0;
        while (gx_objloader_ObjSource[gx_objloader_Index] != '\n'
               && gx_objloader_ObjSource[gx_objloader_Index] != ' ' ) {
            gx_objloader_T[i] = gx_objloader_ObjSource[gx_objloader_Index];
            gx_objloader_Index++;
            i++;
        }

        gx_objloader_T[i] = '\0';
        uvTek->uv.v = (float)atof(gx_objloader_T);

        skipComment();
        skipGaps();
        solveNewLine();

        gx_objloader_Index++;
        if (gx_objloader_ObjSource[gx_objloader_Index] != 't' ) break;
        gx_objloader_Index++;
    }

    uvTek = uvTop; 
    uvTop = uvTop->next; 
    delete uvTek;

    return uvTop;
}

GXOBJTriangle* __stdcall gxParseTriangles()
{
    GXOBJTriangle* tgTop = new GXOBJTriangle;
    GXOBJTriangle* tgTek = tgTop;
    tgTek->next = 0;
    int i = 0;

    while (true) {
        tgTek->next = new GXOBJTriangle;
        tgTek = tgTek->next;
        tgTek->next = 0;

        gx_objloader_TotalTriangles++;

        for (int j = 0; j < 3; j++) {
            skipGaps();
            i = 0;
            while (gx_objloader_ObjSource[gx_objloader_Index] != '/') {
                gx_objloader_T[i] = gx_objloader_ObjSource[gx_objloader_Index];
                gx_objloader_Index++;
                i++;
            }

            gx_objloader_T[i] = '\0';
            tgTek->dat[j].v = atoi(gx_objloader_T);

            gx_objloader_Index++;
            skipGaps();
            i = 0;
            while (gx_objloader_ObjSource[gx_objloader_Index] != '/') {
                gx_objloader_T[i] = gx_objloader_ObjSource[gx_objloader_Index];
                gx_objloader_Index++;
                i++;
            }

            gx_objloader_T[i] = '\0';
            tgTek->dat[j].vt = atoi(gx_objloader_T);

            gx_objloader_Index++;
            skipGaps();
            i = 0;
            while (gx_objloader_ObjSource[gx_objloader_Index] != '\n'
                   && gx_objloader_ObjSource[gx_objloader_Index] != ' ') {
                gx_objloader_T[i] = gx_objloader_ObjSource[gx_objloader_Index];
                gx_objloader_Index++;
                i++;
            }

            gx_objloader_T[i] = '\0';
            tgTek->dat[j].vn = atoi(gx_objloader_T);
        }

        skipComment();
        skipGaps();
        solveNewLine();

        if (gx_objloader_Index > (int)gx_objloader_FileSize - 1) break;
        gx_objloader_Index++;
    }

    tgTek = tgTop;
    tgTop = tgTop->next;
    delete tgTek;

    return tgTop;
}

int __stdcall gxLoadOBJ(const QString fileName, GXOBJPoint** points)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    gx_objloader_FileSize = data.size();
    gx_objloader_ObjSource = data.data();

    setlocale(LC_ALL, "C");

    gx_objloader_TotalVertexes = 0;
    gx_objloader_TotalNormals = 0;
    gx_objloader_TotalUVs = 0;
    gx_objloader_TotalTriangles = 0;

    GXOBJVertex* vTop = 0;
    GXOBJNormals* nTop = 0;
    GXOBJUV_s* uvTop = 0;

    GXOBJTriangle* tgTop = 0;
    GXOBJTriangle* tgTek = 0;

    gx_objloader_Index = 0;

    solveNewLine();

    gx_objloader_Index++;
    vTop = gxParseVertices();

    switch (gx_objloader_ObjSource[gx_objloader_Index]) {
        case 'n': {
            nTop = gxParseNormals();
            break;
        }
        case 't': {
            uvTop = gxParseUVs ();
            break;
        }
        default: {
            // NOTHING
            break;
        }
    }

    switch (gx_objloader_ObjSource[gx_objloader_Index]) {
        case 'n': {
            nTop = gxParseNormals();
            break;
        }
        case 't': {
            uvTop = gxParseUVs();
            break;
        }
        default: {
            // NOTHING
            break;
        }
    }

    gx_objloader_Index++;
    tgTop = gxParseTriangles();

    file.close();

    GXVec3* vertices = (GXVec3*)malloc(gx_objloader_TotalVertexes * sizeof(GXVec3));
    GXOBJVertex* t = vTop;
    for (unsigned int i = 0; i < gx_objloader_TotalVertexes; i++, t = t->next) {
        vertices[i] = t->position;
    }

    deleteList(vTop);

    GXVec3* normals = (GXVec3*)malloc(gx_objloader_TotalNormals * sizeof(GXVec3));
    GXOBJNormals* n = nTop;
    for (unsigned int i = 0; i < gx_objloader_TotalNormals; i++, n = n->next) {
        normals[ i ] = n->normal;
    }

    deleteList(nTop);

    GXVec2* uv_s = (GXVec2*)malloc(gx_objloader_TotalUVs * sizeof(GXVec2));
    GXOBJUV_s* u = uvTop;
    for (unsigned int i = 0; i < gx_objloader_TotalUVs; i++, u = u->next) {
        uv_s[i] = u->uv;
    }

    deleteList(uvTop);

    *points = (GXOBJPoint*)malloc(3 * gx_objloader_TotalTriangles * sizeof(GXOBJPoint));
    GXOBJPoint* outPoints = *points;

    tgTek = tgTop;
    int totalElements = 0;
    while (tgTek != nullptr) {
        for (unsigned char j = 0; j < 3; j++) {
            int offset = tgTek->dat[j].v - 1;
            outPoints[totalElements].vertex = vertices[offset];

            offset = tgTek->dat[j].vt - 1;
            outPoints[totalElements].uv = uv_s[offset];

            offset = tgTek->dat[j].vn - 1;
            outPoints[totalElements].normal = normals[offset];

            totalElements++;
        }
        tgTek = tgTek->next;
    }

    free(vertices);
    free(normals);
    free(uv_s);

    deleteList(tgTop);

    return totalElements;
}

} // namespace gx_common
