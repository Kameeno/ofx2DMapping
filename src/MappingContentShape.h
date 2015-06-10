#pragma once

#include "MappingShape.h"

class MappingContentShape : public MappingShape {

    public:

    ofPoint     src[4];
    float       src_width;
    float       src_height;
    ofMatrix4x4 matrix_src_dst;

    ofFbo_ptr fbo;

    MappingContentShape() {

        this->nature = "CONTENT_SHAPE";

        this->src[0].x = 0;
        this->src[0].y = 0;
        this->src[1].x = 1;
        this->src[1].y = this->src[0].y;
        this->src[2].x = this->src[1].x;
        this->src[2].y = 1;
        this->src[3].x = this->src[0].x;
        this->src[3].y = this->src[2].y;

        this->matrix_src_dst.makeIdentityMatrix();
    }

    MappingContentShape(const MappingContentShape& obj) : MappingShape(obj) {
        for(int i = 0; i < 4; i++) {
            this->src[i] = obj.src[i];
        }
        this->src_width = obj.src_width;
        this->src_height = obj.src_height;
        this->matrix_src_dst = obj.matrix_src_dst;
        this->fbo = obj.fbo;
    }

    ofPtr<MappingObject> clone() const {
        return ofPtr<MappingObject>(new MappingContentShape(*this));
    }

    void loadXml(ofxXmlSettings_ptr xml) {
        MappingShape::loadXml(xml);
        xml->pushTag("src", 0);
            setRectangle(xml, src);
        xml->popTag();
    }

    void saveXml(ofxXmlSettings_ptr xml) {
        MappingShape::saveXml(xml);
        xml->addTag("src");
        xml->pushTag("src", 0);
            xml->addTag("lefttop");
            xml->pushTag("lefttop", 0);
                xml->addValue("x", src[0].x);
                xml->addValue("y", src[0].y);
            xml->popTag();
            xml->addTag("righttop");
            xml->pushTag("righttop", 0);
                xml->addValue("x", src[1].x);
                xml->addValue("y", src[1].y);
            xml->popTag();
            xml->addTag("rightbottom");
            xml->pushTag("rightbottom", 0);
                xml->addValue("x", src[2].x);
                xml->addValue("y", src[2].y);
            xml->popTag();
            xml->addTag("leftbottom");
            xml->pushTag("leftbottom", 0);
                xml->addValue("x", src[3].x);
                xml->addValue("y", src[3].y);
            xml->popTag();
        xml->popTag();
    }

    void update(float w, float h) {
        MappingShape::update(w, h);
        this->findHomography(this->dst, this->src, (GLfloat*) this->matrix_src_dst.getPtr(), true, w, h);
    }

    void setFbo(ofFbo_ptr &fbo) {
        this->fbo = fbo;
        src_width = fbo->getWidth();
        src_height = fbo->getHeight();
    }

    void draw(float w, float h) {

        ofPushMatrix();

            glMultMatrixf(this->matrix_dst_norm.getPtr());

            if(this->fbo) {

                ofSetColor(255);

                this->fbo->getTexture().bind();

                if(this->polyline.size()!= 4) {

                    glBegin(GL_TRIANGLES);
                    for (int i = 0; i < this->triangle.nTriangles; i++){

                        float cx = (((this->triangle.triangles[i].a.x - this->dst[0].x) / (this->dst[1].x-this->dst[0].x) * (this->src[1].x-this->src[0].x))+ this->src[0].x)* fbo->getWidth();
                        float cy = (((this->triangle.triangles[i].a.y - this->dst[0].y) / (this->dst[2].y-this->dst[0].y) * (this->src[2].y-this->src[0].y))+ this->src[0].y) * fbo->getHeight();
                        float ox = (this->triangle.triangles[i].a.x - this->dst[0].x) / (float)(this->dst[1].x-this->dst[0].x) * w;
                        float oy = (this->triangle.triangles[i].a.y - this->dst[0].y) / (float)(this->dst[2].y-this->dst[0].y) * h;
                        glTexCoord2f(cx, cy);
                        glVertex2f(ox, oy);

                        cx = (((this->triangle.triangles[i].b.x - this->dst[0].x) / (this->dst[1].x-this->dst[0].x) * (this->src[1].x-this->src[0].x))+ this->src[0].x)* fbo->getWidth();
                        cy = (((this->triangle.triangles[i].b.y - this->dst[0].y) / (this->dst[2].y-this->dst[0].y) * (this->src[2].y-this->src[0].y))+ this->src[0].y) * fbo->getHeight();
                        ox = (this->triangle.triangles[i].b.x - this->dst[0].x)/(float)(this->dst[1].x-this->dst[0].x)*w;
                        oy = (this->triangle.triangles[i].b.y - this->dst[0].y)/(float)(this->dst[2].y-this->dst[0].y)*h;
                        glTexCoord2f(cx, cy);
                        glVertex2f(ox, oy);

                        cx = (((this->triangle.triangles[i].c.x - this->dst[0].x) / (this->dst[1].x-this->dst[0].x) * (this->src[1].x-this->src[0].x))+ this->src[0].x)* fbo->getWidth();
                        cy = (((this->triangle.triangles[i].c.y - this->dst[0].y) / (this->dst[2].y-this->dst[0].y) * (this->src[2].y-this->src[0].y))+ this->src[0].y) * fbo->getHeight();
                        ox = (this->triangle.triangles[i].c.x - this->dst[0].x)/(float)(this->dst[1].x-this->dst[0].x)*w;
                        oy = (this->triangle.triangles[i].c.y - this->dst[0].y)/(float)(this->dst[2].y-this->dst[0].y)*h;
                        glTexCoord2f(cx, cy);
                        glVertex2f(ox, oy);

                    }
                    glEnd();
                }
                else {

                    glBegin(GL_QUADS);

                    glTexCoord2f(this->src[0].x*fbo->getWidth(), this->src[0].y*fbo->getHeight());		glVertex3f(0, 0, 0);
                    glTexCoord2f(this->src[1].x*fbo->getWidth(), this->src[1].y*fbo->getHeight());		glVertex3f(w, 0, 0);
                    glTexCoord2f(this->src[2].x*fbo->getWidth(), this->src[2].y*fbo->getHeight());		glVertex3f(w, h, 0);
                    glTexCoord2f(this->src[3].x*fbo->getWidth(), this->src[3].y*fbo->getHeight());		glVertex3f(0, h, 0);

                   glEnd();
                }

                this->fbo->getTexture().unbind();

            }

        ofPopMatrix();

    }

    ofTexture* getTexture() {
        if(fbo)
            return &fbo->getTexture();
        else
            return 0;
    }

};
