#include "sxGL.h"
#include "sxGLObject.h"
#include "sxClip.h"

#ifdef SXGL_LIB

inline void glInterpolition(GLenum pairedPlane, GLTypeT sign, GLVertexPosStruct * A, GLVertexPosStruct * B, GLVertexPosStruct * gen) {
    GLTypeT factor;
	GLTypeT one;
	one = GL_TYPET_ONE;
	factor = (sign * A->pos[pairedPlane] - A->pos[3]) / ((B->pos[3] - A->pos[3]) - sign * (B->pos[pairedPlane] - A->pos[pairedPlane]));
    gen->pos[0] = (one - factor) * A->pos[0] + factor * B->pos[0];
    gen->pos[1] = (one - factor) * A->pos[1] + factor * B->pos[1];
    gen->pos[2] = (one - factor) * A->pos[2] + factor * B->pos[2];
    gen->pos[3] = (one - factor) * A->pos[3] + factor * B->pos[3];
}

/************************************************************
** Clip a primitive with specific pair of planes.
** Input: pairedPlanes: 0 - left & right planes, 1 - top & bottom planes, 2 - near & far planes
**        vertices: A pointer to an array of vertex coords
**        vertexNum: number of vertices in vertexs array
**        result: an array used for returning results.
**        resultNum: number of vertices in result array
** Return: None
*************************************************************/
void clipWithPairedPlanes(GLenum pairedPlanes, GLVertexPosStruct * vertices, GLubyte vertexNum, GLVertexPosStruct * result, GLubyte * resultNum) {
	GLTypeT minus_one;
	GLTypeT one;
    GLTypeT zero;
	minus_one = -GL_TYPET_ONE;
	one = GL_TYPET_ONE;
    zero = 0;
    if(vertexNum == 0) {
        *resultNum = 0;
        return;
    }
    /* Check each vertex and see if it's outside of view volume */
    GLubyte maxIndex = vertexNum - 1;
    // near & left & bottom, far & right & top shares this enum
    typedef enum {behindCamera, tooLBN, onLBN, onLBNNoClip, noClip, onRTFNoClip, onRTF, tooRTF} clipPosition_t;
    clipPosition_t needClip[8];
    GLVertexPosStruct * curVertexPointer;
    for(GLubyte i=0; i<= maxIndex; i++) {
        curVertexPointer = &(vertices[i]);
        GLTypeT Wc = curVertexPointer->pos[3];
        if ((pairedPlanes == 2) && ((Wc < zero) || (Wc == zero))) {
            needClip[i] = behindCamera;
        }
        else if(curVertexPointer->pos[pairedPlanes] < -Wc) {
            needClip[i] = tooLBN;
            //sxglDebug("Vertex %d, too left/bottom/near\n", i);
        }
        else if(curVertexPointer->pos[pairedPlanes] > Wc) {
            needClip[i] = tooRTF;
            // sxglDebug("Vertex %d, too right/top/far\n", i);
        }
        else if(curVertexPointer->pos[pairedPlanes] == -Wc) {
            needClip[i] = onLBN;
            // sxglDebug("Vertex %d, on left/bottom/near\n", i);
        }
        else if(curVertexPointer->pos[pairedPlanes] == Wc) {
            needClip[i] = onRTF;
            // sxglDebug("Vertex %d, on right/top/far\n", i);
        }
        else {
            needClip[i] = noClip;
            //sxglDebug("Vertex %d, no clip\n", i);
        }
    }

    /* Check if the whole primitive is clipped */
    GLubyte totalNeedLBNClip = 0;
    GLubyte totalNeedRTFClip = 0;
    for(GLubyte i=0; i<= maxIndex; i++) {
        if(needClip[i] < noClip) {
            totalNeedLBNClip += 1;
        }
        else if(needClip[i] > noClip) {
            totalNeedRTFClip += 1;
        }
    }

    if( (totalNeedLBNClip == vertexNum) || 
        (totalNeedRTFClip == vertexNum)) {
        //sxglDebug("Current primitive is wholely clipped because it's out of view volume.\n");
        *resultNum = 0;
        return;
    }
    else if ((totalNeedLBNClip == 0) && (totalNeedRTFClip == 0)) {
        *resultNum = vertexNum;
        for (GLubyte i = 0; i < vertexNum; i++) {
            result[i] = vertices[i];
        }
        return;
    }
    //sxglDebug("Current primitive is not wholely clipped.\n");

    /* Clip with left/bottom/near plane */
    GLubyte prev;
    GLubyte next;
    GLVertexPosStruct * prevVertexPointer;
    GLVertexPosStruct * nextVertexPointer;
    GLubyte clippedVertexNum = 0;
    GLVertexPosStruct clippedVertexAfterLBN[9];
    clipPosition_t needClipAfterLBN[9];
    
    for(GLubyte i=0; i<= maxIndex; i++) {
        curVertexPointer = &(vertices[i]);
        if(i==maxIndex) {
            prev = maxIndex-1;
            next = 0;
        }
        else if(i==0) {
            prev = maxIndex;
            next = 1;
        }
        else {
            prev = i-1;
            next = i+1;
        }
        prevVertexPointer = &(vertices[prev]);
        nextVertexPointer = &(vertices[next]);
        if(needClip[i] >= noClip) {
            if(needClip[prev] == onLBN) {
                needClip[prev] = onLBNNoClip;
                clippedVertexAfterLBN[clippedVertexNum] = *prevVertexPointer;
                needClipAfterLBN[clippedVertexNum] = onLBNNoClip;
                clippedVertexNum += 1;
            }
            else if(needClip[prev] <= tooLBN) {
                glInterpolition(pairedPlanes, minus_one, prevVertexPointer, curVertexPointer, &(clippedVertexAfterLBN[clippedVertexNum]));
                needClipAfterLBN[clippedVertexNum] = onLBNNoClip;
                clippedVertexNum += 1;
            }
            clippedVertexAfterLBN[clippedVertexNum] = *curVertexPointer;
            needClipAfterLBN[clippedVertexNum] = needClip[i];
            clippedVertexNum += 1;
            if(needClip[next] == onLBN) {
                needClip[next] = onLBNNoClip;
                clippedVertexAfterLBN[clippedVertexNum] = *nextVertexPointer;
                needClipAfterLBN[clippedVertexNum] = onLBNNoClip;
                clippedVertexNum += 1;
            }
            else if(needClip[next] <= tooLBN) {
                glInterpolition(pairedPlanes, minus_one, nextVertexPointer, curVertexPointer, &(clippedVertexAfterLBN[clippedVertexNum]));
                needClipAfterLBN[clippedVertexNum] = onLBNNoClip;
                clippedVertexNum += 1;
            }
        }
    }
    //for (GLubyte i = 0; i < clippedVertexNum; i++) {
    //    sxglDebug("Vertex %d after left/bottom/near plane clip: %f %f %f %f\n", i, clippedVertexAfterLBN[i].position[0], clippedVertexAfterLBN[i].position[1], clippedVertexAfterLBN[i].position[2], clippedVertexAfterLBN[i].position[3]);
    //}
    
    /* Clip with right/top/far plane */
    maxIndex = clippedVertexNum - 1;
    if((pairedPlanes == 2) && (pGLContext->state.GLEnableState.bit.GL_STATE_CLIP_FAR_PLANE == 0)) {
        *resultNum = clippedVertexNum;
        for(GLubyte i = 0; i <= maxIndex; i++) {
            result[i] = clippedVertexAfterLBN[i];
        }
    }
    else {
        *resultNum = 0;
        for(GLubyte i=0; i<= maxIndex; i++) {
            curVertexPointer = &(clippedVertexAfterLBN[i]);
            if(i==maxIndex) {
                prev = maxIndex-1;
                next = 0;
            }
            else if(i==0) {
                prev = maxIndex;
                next = 1;
            }
            else {
                prev = i-1;
                next = i+1;
            }
            prevVertexPointer = &(clippedVertexAfterLBN[prev]);
            nextVertexPointer = &(clippedVertexAfterLBN[next]);
            if (needClipAfterLBN[i] <= noClip) {
                if(needClipAfterLBN[prev] == onRTF) {
                    needClipAfterLBN[prev] = onRTFNoClip;
                    result[*resultNum] = *prevVertexPointer;
                    *resultNum += 1;
                }
                else if(needClipAfterLBN[prev] >= tooRTF) {
                    glInterpolition(pairedPlanes, one, prevVertexPointer, curVertexPointer, &(result[*resultNum]));
                    *resultNum += 1;
                }
                result[*resultNum] = *curVertexPointer;
                *resultNum += 1;
                if(needClipAfterLBN[next] == onRTF) {
                    needClipAfterLBN[next] = onRTFNoClip;
                    result[*resultNum] = *nextVertexPointer;
                    *resultNum += 1;
                }
                else if(needClipAfterLBN[next] >= tooRTF) {
                    glInterpolition(pairedPlanes, one, curVertexPointer, nextVertexPointer, &(result[*resultNum]));
                    *resultNum += 1;
                }
            }
        }
        // for (GLubyte i = 0; i < *resultNum; i++) {
        //    sxglDebug("Vertex %d after right/top/far plane clip: %f %f %f %f\n", i, result[i].pos[0], result[i].pos[1], result[i].pos[2], result[i].pos[3]);
        // }
    }
}

/************************************************************
** Clip current primitive of current active context.
** Input: A pointer to an array including 4 primitives,
**        used for returning results.
** Return: None
*************************************************************/
void glClipCurrentPrimitive(GLPrimitiveVertexInfo* clippedPrimitives) {
    //assert(clippedPrimitives!=NULL);
    //assert(pGLContext!=NULL);

    // TODO
    // if(pGLContext->mode == SXGL_DRAW_2D) {
    //     *clippedPrimitives = pGLContext->scene.curPrimitive;
    //     (clippedPrimitives+1)->numOfVertex = 0;
    //     return;
    // }

    if (pGLContext->scene.curPrimitive.vertexInfo[0].numOfVertex == 0) {
        clippedPrimitives[0].numOfVertex = 0;
        clippedPrimitives[1].numOfVertex = 0;
        clippedPrimitives[2].numOfVertex = 0;
        clippedPrimitives[3].numOfVertex = 0;
        return;
    }

    /* Clip with near & far plane */
    GLVertexPosStruct clippedVertexAfterNearFar[6];
    GLubyte clippedVertexNum = 0;

    clipWithPairedPlanes(2, pGLContext->scene.curPrimitive.vertexInfo->vertex, pGLContext->scene.curPrimitive.vertexInfo->numOfVertex, clippedVertexAfterNearFar, &clippedVertexNum);
    if (clippedVertexNum == 0) {
        clippedPrimitives[0].numOfVertex = 0;
        clippedPrimitives[1].numOfVertex = 0;
        clippedPrimitives[2].numOfVertex = 0;
        clippedPrimitives[3].numOfVertex = 0;
        return;
    }

    GLVertexPosStruct* clipFinalResult;
    if (pGLContext->state.GLEnableState.bit.GL_STATE_CLIP_LRTB_PLANE != 0) {
        /* Clip with left & right plane */
        GLVertexPosStruct clippedVertexAfterLeftRight[8];
        clipWithPairedPlanes(0, clippedVertexAfterNearFar, clippedVertexNum, clippedVertexAfterLeftRight, &clippedVertexNum);
        if (clippedVertexNum == 0) {
            clippedPrimitives[0].numOfVertex = 0;
            clippedPrimitives[1].numOfVertex = 0;
            clippedPrimitives[2].numOfVertex = 0;
            clippedPrimitives[3].numOfVertex = 0;
            return;
        }
        /* Clip with top & bottom plane */
        GLVertexPosStruct clippedVertexAfterTopBottom[10];
        clipWithPairedPlanes(1, clippedVertexAfterLeftRight, clippedVertexNum, clippedVertexAfterTopBottom, &clippedVertexNum);
        if (clippedVertexNum == 0) {
            clippedPrimitives[0].numOfVertex = 0;
            clippedPrimitives[1].numOfVertex = 0;
            clippedPrimitives[2].numOfVertex = 0;
            clippedPrimitives[3].numOfVertex = 0;
            return;
        }
        clipFinalResult = clippedVertexAfterTopBottom;
    }
    else {
        clipFinalResult = clippedVertexAfterNearFar;
    }

    /* Perspective division */
    for (GLubyte i = 0; i < clippedVertexNum; i++) {
        clipFinalResult[i].pos[0] = clipFinalResult[i].pos[0] / clipFinalResult[i].pos[3];
        clipFinalResult[i].pos[1] = clipFinalResult[i].pos[1] / clipFinalResult[i].pos[3];
        clipFinalResult[i].pos[2] = clipFinalResult[i].pos[2] / clipFinalResult[i].pos[3];
        clipFinalResult[i].pos[3] = GL_TYPET_ONE;
        //sxglDebug("Vertex %d after perspective division: %f, %f, %f, %f\n", i, clipFinalResult[i].pos[0], clipFinalResult[i].pos[1], clipFinalResult[i].pos[2], clipFinalResult[i].pos[3]);
    }

    /* Primitive assembly */
    clippedPrimitives[0].numOfVertex = 0;
    clippedPrimitives[1].vertex[0] = clipFinalResult[0];
    clippedPrimitives[2].vertex[0] = clipFinalResult[0];
    clippedPrimitives[3].vertex[0] = clipFinalResult[0];
    for (GLubyte i = 0; (i < 4) && (i < clippedVertexNum); i++) {
        clippedPrimitives[0].numOfVertex++;
        clippedPrimitives[0].vertex[i] = clipFinalResult[i];
    }
    for (GLubyte i = 1; i < 4; i++) {
        clippedPrimitives[i].numOfVertex = 0;
        if (clippedVertexNum > 2 * i + 2) {
            clippedPrimitives[i].numOfVertex++;
            clippedPrimitives[i].vertex[0] = clipFinalResult[0];
            for (GLubyte j = 1; (j < 4) && (2 * i + j < clippedVertexNum); j++) {
                clippedPrimitives[i].numOfVertex++;
                clippedPrimitives[i].vertex[j] = clipFinalResult[2 * i + j];
            }
        }
    }
}

#endif