#pragma once

typedef enum {
    TEXTURE_PROP,
    COLOR_PROP
} PropType;

typedef struct {
    PropType type;
    char* name;
    Texture* texture;
    Color color;
} Property;

typedef struct {
    Mesh* mesh;
    Shader* shader;
    float pos_x;
    float pos_y;
    float pos_z;
    float rot_x;
    float rot_y;
    float rot_z;
    float scl_x;
    float scl_y;
    float scl_z;
    Property** properties;
} Object;

char* vertex_2D = "#version 330 core\nprecision mediump float;\nprecision mediump int;\n\nuniform vec3 pos;\nuniform vec3 rot;\nuniform vec3 scl;\nuniform vec3 cam_pos;\nuniform vec3 cam_rot;\nuniform float aspect;\n\nin vec3 in_pos;\nin vec2 in_coord;\nin vec3 in_normal;\nout vec2 coord;\nout vec3 normal;\n\nvoid main() {\n    normal = vec3(cos(rot.z / 360.0f * 6.282f) * in_normal.x + sin(rot.z / 360.0f * 6.282f) * in_normal.y, cos(rot.z / 360.0f * 6.282f) * in_normal.y - sin(rot.z / 360.0f * 6.282f) * in_normal.x, in_normal.z);\n    coord = in_coord;\n    vec2 position = in_pos.xy * scl.xy;\n    position = vec2(cos(rot.z / 360.0f * 6.282f) * position.x + sin(rot.z / 360.0f * 6.282f) * position.y, cos(rot.z / 360.0f * 6.282f) * position.y - sin(rot.z / 360.0f * 6.282f) * position.x) + pos.xy - cam_pos.xy;\n    position = vec2(cos(-cam_rot.z / 360.0f * 6.282f) * position.x + sin(-cam_rot.z / 360.0f * 6.282f) * position.y, cos(-cam_rot.z / 360.0f * 6.282f) * position.y - sin(-cam_rot.z / 360.0f * 6.282f) * position.x);\n    gl_Position = vec4(position.x * cam_pos.z, aspect * position.y * cam_pos.z, -1.0f, 1.0f);\n}";
char* vertex_3D = "#version 330 core\nprecision mediump float;\nprecision mediump int;\n\nuniform vec3 pos;\nuniform vec3 rot;\nuniform vec3 scl;\nuniform vec3 cam_pos;\nuniform vec3 cam_rot;\nuniform float aspect;\n\nin vec3 in_pos;\nin vec2 in_coord;\nin vec3 in_normal;\nout vec2 coord;\nout vec3 normal;\n\nvoid main() {\n    vec3 n = vec3(in_normal.x, cos(rot.x / 360.0f * 6.282f) * in_normal.y + sin(rot.x / 360.0f * 6.282f) * in_normal.z, cos(rot.x / 360.0f * 6.282f) * in_normal.z - sin(rot.x / 360.0f * 6.282f) * in_normal.y);\nn = vec3(cos(rot.y / 360.0f * 6.282f) * n.x + sin(rot.y / 360.0f * 6.282f) * n.z, n.y, cos(rot.y / 360.0f * 6.282f) * n.z - sin(rot.y / 360.0f * 6.282f) * n.x);\nn = vec3(cos(rot.z / 360.0f * 6.282f) * n.x + sin(rot.z / 360.0f * 6.282f) * n.y, cos(rot.z / 360.0f * 6.282f) * n.y - sin(rot.z / 360.0f * 6.282f) * n.x, n.z);\nnormal = n;\n    coord = in_coord;\n    vec3 position = in_pos * scl;\n    position = vec3(position.x, cos(rot.x / 360.0f * 6.282f) * position.y + sin(rot.x / 360.0f * 6.282f) * position.z, cos(rot.x / 360.0f * 6.282f) * position.z - sin(rot.x / 360.0f * 6.282f) * position.y);\n    position = vec3(cos(rot.y / 360.0f * 6.282f) * position.x + sin(rot.y / 360.0f * 6.282f) * position.z, position.y, cos(rot.y / 360.0f * 6.282f) * position.z - sin(rot.y / 360.0f * 6.282f) * position.x);\n    position = vec3(cos(rot.z / 360.0f * 6.282f) * position.x + sin(rot.z / 360.0f * 6.282f) * position.y, cos(rot.z / 360.0f * 6.282f) * position.y - sin(rot.z / 360.0f * 6.282f) * position.x, position.z);\n    position = position + pos - cam_pos;\n    position = vec3(position.x, cos(-cam_rot.x / 360.0f * 6.282f) * position.y + sin(-cam_rot.x / 360.0f * 6.282f) * position.z, cos(-cam_rot.x / 360.0f * 6.282f) * position.z - sin(-cam_rot.x / 360.0f * 6.282f) * position.y);\n    position = vec3(cos(-cam_rot.y / 360.0f * 6.282f) * position.x + sin(-cam_rot.y / 360.0f * 6.282f) * position.z, position.y, cos(-cam_rot.y / 360.0f * 6.282f) * position.z - sin(-cam_rot.y / 360.0f * 6.282f) * position.x);\n    position = vec3(position.x, cos(-cam_rot.z / 360.0f * 6.282f) * position.y + sin(-cam_rot.z / 360.0f * 6.282f) * position.z, cos(-cam_rot.z / 360.0f * 6.282f) * position.z - sin(-cam_rot.z / 360.0f * 6.282f) * position.y);\n    gl_Position = vec4(position.x, aspect * position.y, position.z / 1000.0f * 2.0f - 1.0f, position.z);\n}";

char* fragment_basic = "#version 330 core\nprecision mediump float;\nprecision mediump int;\n\nuniform vec3 col;\n\nin vec2 coord;\nin vec3 normal;\nout vec4 out_color;\n\nvoid main() {\n    out_color = vec4(col, 1.0f);\n}";
char* fragment_textured = "#version 330 core\nprecision mediump float;\nprecision mediump int;\n\nuniform sampler2D tex;\n\nin vec2 coord;\nin vec3 normal;\nout vec4 out_color;\n\nvoid main() {\n    if(texture(tex, coord).a == 0.0f) discard;\n    out_color = texture(tex, coord);\n}";
char* fragment_basic_shaded = "#version 330 core\nprecision mediump float;\nprecision mediump int;\n\nuniform vec3 col;\n\nin vec2 coord;\nin vec3 normal;\nout vec4 out_color;\n\nvoid main() {\n    out_color = clamp(dot(-normal, normalize(vec3(1.0f, -1.0f, 1.0f))) / 2.0f + 0.5f, 0.2f, 1.0f) * vec4(col, 1.0f);\n}";
char* fragment_textured_shaded = "#version 330 core\nprecision mediump float;\nprecision mediump int;\n\nuniform sampler2D tex;\n\nin vec2 coord;\nin vec3 normal;\nout vec4 out_color;\n\nvoid main() {\n    if(texture(tex, coord).a == 0.0f) discard;\n    out_color = clamp(dot(-normal, normalize(vec3(1.0f, -1.0f, 1.0f))) / 2.0f + 0.5f, 0.2f, 1.0f) * texture(tex, coord);\n}";

float cam_pos_x = 0.0f;
float cam_pos_y = 0.0f;
float cam_pos_z = 0.0f;
float cam_rot_x = 0.0f;
float cam_rot_y = 0.0f;
float cam_rot_z = 0.0f;

Object* objects[1024];

void setTextureProperty(Object* object, char* name, Texture* texture) {
    Property* property = malloc(sizeof(Property));

    property->type = TEXTURE_PROP;
    property->name = name;
    property->texture = texture;

    for(int i = 0; i < 32; i++) {
        if(object->properties[i] == NULL) {
            object->properties[i] = property;
            break;
        }
    }

    //setUniformTexture(object->shader, property, texture, num);
}

void setColorProperty(Object* object, char* name, Color color) {
    Property* property = malloc(sizeof(Property));

    property->type = COLOR_PROP;
    property->name = name;
    property->color = color;

    for(int i = 0; i < 32; i++) {
        if(object->properties[i] == NULL) {
            object->properties[i] = property;
            break;
        }
    }

    //setUniformVec3(object->shader, property, r, g, b);
}

Object* createObject(Mesh* mesh, char* vertex, char* fragment, 
        float pos_x, float pos_y, float pos_z, 
        float rot_x, float rot_y, float rot_z,
        float scl_x, float scl_y, float scl_z) {
    Object* object = malloc(sizeof(Object));

    object->mesh = mesh;
    object->shader = createShaderRaw(vertex, fragment);
    object->pos_x = pos_x;
    object->pos_y = pos_y;
    object->pos_z = pos_z;
    object->rot_x = rot_x;
    object->rot_y = rot_y;
    object->rot_z = rot_z;
    object->scl_x = scl_x;
    object->scl_y = scl_y;
    object->scl_z = scl_z;

    object->properties = malloc(sizeof(Property*) * 32);

    for(int i = 0; i < 32; i++) {
        object->properties[i] = 0;
    }

    for(int i = 0; i < 1024; i++) {
        if(objects[i] == NULL) {
            objects[i] = object;
            break;
        }
    }

    return object;
}

Object* createObject2D(Mesh* mesh, char* fragment, float pos_x, float pos_y, float rot_z, float scl_x, float scl_y) {
    return createObject(mesh, vertex_2D, fragment, pos_x, pos_y, 0.0f, 0.0f, 0.0f, rot_z, scl_x, scl_y, 0.0f);
}

Object* createBasic2D(Mesh* mesh, float pos_x, float pos_y, float rot_z, float scl_x, float scl_y) {
    return createObject2D(mesh, fragment_basic, pos_x, pos_y, rot_z, scl_x, scl_y);
}

Object* createTextured2D(Mesh* mesh, float pos_x, float pos_y, float rot_z, float scl_x, float scl_y) {
    return createObject2D(mesh, fragment_textured, pos_x, pos_y, rot_z, scl_x, scl_y);
}

Object* createBasicShaded2D(Mesh* mesh, float pos_x, float pos_y, float rot_z, float scl_x, float scl_y) {
    return createObject2D(mesh, fragment_basic_shaded, pos_x, pos_y, rot_z, scl_x, scl_y);
}

Object* createTexturedShaded2D(Mesh* mesh, float pos_x, float pos_y, float rot_z, float scl_x, float scl_y) {
    return createObject2D(mesh, fragment_textured_shaded, pos_x, pos_y, rot_z, scl_x, scl_y);
}

Object* createObject3D(Mesh* mesh, char* fragment, 
        float pos_x, float pos_y, float pos_z, 
        float rot_x, float rot_y, float rot_z,
        float scl_x, float scl_y, float scl_z) {
    return createObject(mesh, vertex_3D, fragment, pos_x, pos_y, pos_z, rot_x, rot_y, rot_z, scl_x, scl_y, scl_z);
}

Object* createBasic3D(Mesh* mesh, 
        float pos_x, float pos_y, float pos_z,
        float rot_x, float rot_y, float rot_z,
        float scl_x, float scl_y, float scl_z) {
    return createObject3D(mesh, fragment_basic, pos_x, pos_y, pos_z, rot_x, rot_y, rot_z, scl_x, scl_y, scl_z);
}

Object* createTextured3D(Mesh* mesh,
        float pos_x, float pos_y, float pos_z,
        float rot_x, float rot_y, float rot_z,
        float scl_x, float scl_y, float scl_z) {
    return createObject3D(mesh, fragment_textured, pos_x, pos_y, pos_z, rot_x, rot_y, rot_z, scl_x, scl_y, scl_z);
}

Object* createBasicShaded3D(Mesh* mesh, 
        float pos_x, float pos_y, float pos_z,
        float rot_x, float rot_y, float rot_z,
        float scl_x, float scl_y, float scl_z) {
    return createObject3D(mesh, fragment_basic_shaded, pos_x, pos_y, pos_z, rot_x, rot_y, rot_z, scl_x, scl_y, scl_z);
}

Object* createTexturedShaded3D(Mesh* mesh,
        float pos_x, float pos_y, float pos_z,
        float rot_x, float rot_y, float rot_z,
        float scl_x, float scl_y, float scl_z) {
    return createObject3D(mesh, fragment_textured_shaded, pos_x, pos_y, pos_z, rot_x, rot_y, rot_z, scl_x, scl_y, scl_z);
}

void renderObject(Object* object) {
    if(object == NULL) return;

    int nextTexIndex = 0;

    for(int i = 0; i < 32; i++) {
        if(object->properties[i] == 0) {
            break;
        }

        switch(object->properties[i]->type) {
            case(TEXTURE_PROP): 
                setUniformTexture(
                    object->shader, 
                    object->properties[i]->name, 
                    object->properties[i]->texture,
                    nextTexIndex
                );
                nextTexIndex++;
                break;
            case(COLOR_PROP):
                setUniformVec3(
                    object->shader,
                    object->properties[i]->name,
                    object->properties[i]->color.r,
                    object->properties[i]->color.g,
                    object->properties[i]->color.b
                );
                break;
        }
    }

    setUniformVec3(object->shader, "pos", object->pos_x, object->pos_y, object->pos_z);
    setUniformVec3(object->shader, "rot", object->rot_x, object->rot_y, object->rot_z);
    setUniformVec3(object->shader, "scl", object->scl_x, object->scl_y, object->scl_z);
    setUniformVec3(object->shader, "cam_pos", cam_pos_x, cam_pos_y, cam_pos_z);
    setUniformVec3(object->shader, "cam_rot", cam_rot_x, cam_rot_y, cam_rot_z);
    setUniformFloat(object->shader, "aspect", (float)width / (float)height);
    renderMesh(object->mesh, object->shader);
}

void render() {
    for(int i = 0; i < 256; i++) {
        if(viewports[i] == 0) continue;

        glBindFramebuffer(GL_FRAMEBUFFER, viewports[i]->fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(int j = 0; j < 1024; j++) {
            renderObject(objects[j]);
        }

        glCopyImageSubData(viewports[i]->texture->texture, GL_TEXTURE_2D, 0, 0, 0, 0, 
                viewports[i]->texture2->texture, GL_TEXTURE_2D, 0, 0, 0, 0,
                width, height, 1);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    for(int i = 0; i < 1024; i++) {
        renderObject(objects[i]);
    }
}