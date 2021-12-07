/*
 * Copyright 2006, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define LOG_TAG "webcoreglue"

#include "config.h"
#include "WebViewCore.h"

#include "AccessibilityObject.h"
#include "AndroidHitTestResult.h"
#include "ApplicationCacheStorage.h"//4.2 Merge
#include "Attribute.h"
#include "content/address_detector.h"
#include "Chrome.h"
#include "ChromeClientAndroid.h"
#include "ChromiumIncludes.h"
#include "ClientRect.h"
#include "ClientRectList.h"
#include "Color.h"
#include "CSSPropertyNames.h"
#include "CSSValueKeywords.h"
#include "DatabaseTracker.h"
#include "Document.h"
#include "DocumentMarkerController.h"
#include "DOMWindow.h"
#include "DOMSelection.h"
#include "Element.h"
#include "Editor.h"
#include "EditorClientAndroid.h"
#include "EventHandler.h"
#include "EventNames.h"
#include "ExceptionCode.h"
#include "FocusController.h"
#include "Font.h"
#include "FontCache.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "FrameLoaderClientAndroid.h"
#include "FrameTree.h"
#include "FrameView.h"
#include "Geolocation.h"
#include "GraphicsContext.h"
#include "GraphicsJNI.h"
//#include "GraphicsOperationCollection.h"//4.2 Merge removed in 4.2
#include "HTMLAnchorElement.h"
#include "HTMLAreaElement.h"
#include "HTMLElement.h"
#include "HTMLFormControlElement.h"
#include "HTMLImageElement.h"
#include "HTMLInputElement.h"
#include "HTMLLabelElement.h"
//SAMSUNG CHANGE +
#include "HTMLLinkElement.h"
//SAMSUNG CHANGE -
#include "HTMLMapElement.h"
#include "HTMLNames.h"
#include "WMLNames.h"
#include "HTMLOptGroupElement.h"
#include "HTMLOptionElement.h"
#include "HTMLSelectElement.h"
#include "HTMLTextAreaElement.h"
#include "HistoryItem.h"
#include "HitTestRequest.h"
#include "HitTestResult.h"
#include "InlineTextBox.h"
#include "KeyboardEvent.h"
#include "MemoryUsage.h"
#include "NamedNodeMap.h"
#include "Navigator.h"
#include "Node.h"
#include "NodeList.h"
#if ENABLE(WML)
#include "WMLOptGroupElement.h"
#include "WMLOptionElement.h"
#include "WMLSelectElement.h"
#include "WMLInputElement.h"
#include "WMLNames.h"
#endif

// Samsung Change - HTML5 Web Notification	>>
#if ENABLE(NOTIFICATIONS)
#include "Notification.h"
#include "NotificationPresenter.h"
#endif
// Samsung Change - HTML5 Web Notification	<<
#include "Page.h"
#include "PageGroup.h"
#include "PictureLayerContent.h"
#include "PicturePileLayerContent.h"
#include "PlatformKeyboardEvent.h"
#include "PlatformString.h"
#include "PluginWidgetAndroid.h"
#include "PluginView.h"
#include "Position.h"
#include "ProgressTracker.h"
#include "Range.h"
#include "RenderBox.h"
#include "RenderImage.h"
#include "RenderInline.h"
#include "RenderLayer.h"
#include "RenderPart.h"
#include "RenderText.h"
#include "RenderTextControl.h"
#include "RenderThemeAndroid.h"
#include "RenderView.h"
//SAMSUNG_SHANGE [MPSG100005849] [P120801-5124] ++
#include "RenderMenuList.h" 
//SAMSUNG_SHANGE [MPSG100005849] [P120801-5124] --
#include "ResourceRequest.h"
#include "RuntimeEnabledFeatures.h"
#include "SchemeRegistry.h"
#include "ScopedLocalRef.h"
#include "ScriptController.h"
#include "SelectionController.h"
#include "SelectText.h"
#include "Settings.h"
#include "SkANP.h"
#include "SkTemplates.h"
#include "SkTDArray.h"
#include "SkTypes.h"
#include "SkCanvas.h"
#include "SkGraphics.h"
#include "SkPicture.h"
#include "SkUtils.h"
#include "Text.h"
#include "TextIterator.h"
#include "TilesManager.h"
#include "TypingCommand.h"
#include "WebCache.h"
#include "WebCoreFrameBridge.h"
#include "WebCoreJni.h"
#include "WebFrameView.h"
#include "WindowsKeyboardCodes.h"
//#include "android_graphics.h"//4.2 Merge : removed in 4.2
#include "autofill/WebAutofill.h"
#include "htmlediting.h"
#include "markup.h"
#include "unicode/uloc.h"//4.2 Merge
//SISO_HTMLComposer start
#include "RemoveNodeCommand.h"
#define EMOJI_LOWER_VALUE 127488
#define EMOJI_UPPER_VALUE 128709
//SISO_HTMLComposer end

#include "visible_units.h"

#include <JNIHelp.h>
#include <JNIUtility.h>
#include <androidfw/KeycodeLabels.h>
#include <cutils/properties.h>
#include <v8.h>
#include <wtf/CurrentTime.h>
#include <wtf/text/AtomicString.h>
//SAMSUNG_WEB_WORKER_CHANGES >>
#include "V8Binding.h"
//SAMSUNG_WEB_WORKER_CHANGES <<
#include <wtf/text/CString.h>
#include <wtf/text/StringImpl.h>

#if DEBUG_NAV_UI
#include "SkTime.h"
#endif

#if ENABLE(TOUCH_EVENTS) // Android
#include "PlatformTouchEvent.h"
#endif

#ifdef ANDROID_DOM_LOGGING
#include "AndroidLog.h"
#include "RenderTreeAsText.h"
#include <wtf/text/CString.h>

FILE* gDomTreeFile = 0;
FILE* gRenderTreeFile = 0;
#endif

#include "BaseLayerAndroid.h"

#if USE(ACCELERATED_COMPOSITING)
#include "GraphicsLayerAndroid.h"
#include "RenderLayerCompositor.h"
#endif

#define FOREGROUND_TIMER_INTERVAL 0.004 // 4ms
#define BACKGROUND_TIMER_INTERVAL 1.0 // 1s
#define MAX_HEIGHT 503395  //Form The MAX Email Size.

// How many ms to wait for the scroll to "settle" before we will consider doing
// prerenders
#define PRERENDER_AFTER_SCROLL_DELAY 750

#define TOUCH_FLAG_HIT_HANDLER 0x1
#define TOUCH_FLAG_PREVENT_DEFAULT 0x2

//SAMSUNG ADVANCED TEXT SELECTION - BEGIN
#include <visible_units.h>
#include "TextIterator.h"
//SAMSUNG ADVANCED TEXT SELECTION - END
////////////////////////////////////////////////////////////////////////////////////////////////
//SISO_HTMLComposer start
namespace WebCore {
    extern WTF::String createLocalResource(WebCore::Frame* frame , WTF::String url);
    extern bool saveCachedImageToFile(WebCore::Frame* frame, WTF::String imageUrl, WTF::String filePath);;
    extern void copyImagePathToClipboard(const WTF::String& imagePath);
    extern android::WebHTMLMarkupData* createFullMarkup(const Node* node,const WTF::String& basePath = WTF::String());
}
static const UChar NonBreakingSpaceCharacter = 0xA0;
static const UChar SpaceCharacter = ' ';
//SISO_HTMLComposer end


namespace android {

// SAMSUNG CHANGES : SmartClip >>
Node* WebViewCore::findMinContainerNode(Node* pNode1, Node* pNode2)
{
    if(pNode1 != NULL && pNode2 == NULL)
        return pNode1;
        
    if(pNode1 == NULL && pNode2 != NULL)
        return pNode2;
            
    if(pNode1 == NULL && pNode2 == NULL)
        return NULL;
        
    IntRect rect1 = pNode1->getRect();
    IntRect rect2 = pNode2->getRect();

    if(rect1.contains(rect2) == true)
        return pNode1;

    if(rect2.contains(rect1) == true)
        return pNode2;

    Node* pCurNode = pNode1;
    while(pCurNode != NULL)
    {
        IntRect curRect = pCurNode->getRect();
        if(curRect.contains(rect2) == true)
        {
            return pCurNode;
        }
        pCurNode = pCurNode->parentNode();
    }

    return NULL;
}

String WebViewCore::extractSmartClipData(IntRect checkRect)
{
    Frame* pFrame = focusedFrame();
    String resultStr("");
    Vector<CroppedNodeInfo>* nodeInfoTable = new Vector<CroppedNodeInfo>();
    
    if (pFrame != NULL && nodeInfoTable != NULL)
    {
        Node* pBodyNode = pFrame->document()->body();
        Node* pNode = pBodyNode;
        Node* pMinContainer = NULL;

        // Calculate absolute document coordinate
        IntRect absCheckRect = checkRect;
        absCheckRect.move(m_scrollOffsetX, m_scrollOffsetY);
    
        while(pNode != NULL)
        {
            IntRect nodeRect = pNode->getRect();
            Node::NodeType nodeType  = pNode->nodeType();
            RenderObject*  pRenderer = pNode->renderer();
      
            if(pRenderer != NULL && nodeRect.width() > 0 && nodeRect.height() > 0)
            {
                if(pRenderer->isText() || pRenderer->isRenderImage() )
                {
                    if(absCheckRect.intersects(nodeRect) == true)
                    {
                        pMinContainer = findMinContainerNode(pMinContainer, pNode);    

//                        nodeInfoTable->append(CroppedNodeInfo(pNode, nodeRect));        // Not needs yet.
                    }
                    else
                    {
                        pNode = pNode->traverseNextSibling(pBodyNode);
                        continue;
                    }
                }    
            }
            
            pNode = pNode->traverseNextNode(pBodyNode);
        }    

        // Set entire cropped rect & extract text which included in cropped rect.
        if (pMinContainer != NULL)
        {
            IntRect containerRect = pMinContainer->getRect();
            String  collectedText = extractAllTextFromNode(pMinContainer);
            
            nodeInfoTable->append(CroppedNodeInfo(pMinContainer, containerRect, collectedText));
        }
        // Converts CroppedNodeInfo array to string which will transfer to Java layer.
        resultStr = convertCroppedNodeInfoTableToString(nodeInfoTable);
    }

    if (nodeInfoTable != NULL)    
        delete nodeInfoTable;

    return resultStr;
}


String WebViewCore::convertCroppedNodeInfoTableToString(Vector<CroppedNodeInfo>* nodeInfoTable)
{
    String FieldSeparator;
    String RowSeparator;
    String resultStr;
    int tableSize = nodeInfoTable->size();

    FieldSeparator.append((UChar)0xFFFE);
    RowSeparator  .append((UChar)0xFFFF);

    for (int i=0; i<tableSize; i++)
    {
        CroppedNodeInfo& nodeInfo = (*nodeInfoTable)[i];
        
        String rowStr;
        rowStr = String::number(nodeInfo.m_rect.x()) 
                    + FieldSeparator 
                    + String::number(nodeInfo.m_rect.y()) 
                    + FieldSeparator 
                    + String::number(nodeInfo.m_rect.width()) 
                    + FieldSeparator 
                    + String::number(nodeInfo.m_rect.height()) 
                    + FieldSeparator 
                    + nodeInfo.m_string 
                    + RowSeparator;
                    
        resultStr += rowStr;
    }

    return resultStr;
}


String WebViewCore::extractAllTextFromNode(Node* pNode)
{
    String resultStr;
    Node* pCurNode = pNode;
    int prevYPos = -99999;
    
    while(pCurNode != NULL)
    {
        Node::NodeType nodeType  = pCurNode->nodeType();
        IntRect        nodeRect  = pCurNode->getRect();            
        RenderObject*  pRenderer = pCurNode->renderer();
        
        if(pRenderer != NULL && nodeRect.width() > 0 && nodeRect.height() > 0)
        {
            if(nodeType == Node::TEXT_NODE)
            {
                String nodeValue = pCurNode->nodeValue();

                if(nodeValue.isNull() == true) 
                    nodeValue = "";

                if(nodeValue == "\n")
                    nodeValue = "";

                if(nodeRect.y() != prevYPos) 
                {
                    resultStr += String("\n") + nodeValue;
                    prevYPos = nodeRect.y();
                }
                else
                {
                    resultStr += nodeValue + " ";
                }
            }
        }    

        pCurNode = pCurNode->traverseNextNode(pNode);
    }

    return resultStr;
}


int WebViewCore::getParentCount(Node* pNode)
{
    int parentCount = 0;
    Node* pCurNode = pNode;
    
    while(pCurNode != NULL)
    {
        parentCount++;
        pCurNode = pCurNode->parentNode();
    }

    return parentCount;
}



String WebViewCore::getRectString(IntRect rect)
{
    String result = String::format("%4d, %4d, %4d, %4d", rect.x(), rect.y(), rect.width(), rect.height() );
    return result;
}


String WebViewCore::getNodeTypeStr(Node::NodeType nodeType)
{
    switch(nodeType)
    {
        case Node::ELEMENT_NODE                   : return String("ELEMENT");
        case Node::ATTRIBUTE_NODE                 : return String("ATTRIBUTE");
        case Node::TEXT_NODE                      : return String("TEXT   ");
        case Node::CDATA_SECTION_NODE             : return String("CDATA_SECTION");
        case Node::ENTITY_REFERENCE_NODE          : return String("ENTITY_REFERENCE");
        case Node::ENTITY_NODE                    : return String("ENTITY");
        case Node::PROCESSING_INSTRUCTION_NODE    : return String("PROCESSING_INSTRUCTION");
        case Node::COMMENT_NODE                   : return String("COMMENT");
        case Node::DOCUMENT_NODE                  : return String("DOCUMENT");
        case Node::DOCUMENT_TYPE_NODE             : return String("DOCUMENT_TYPE");
        case Node::DOCUMENT_FRAGMENT_NODE         : return String("DOCUMENT_FRAGMENT");
        case Node::NOTATION_NODE                  : return String("NOTATION");
        case Node::XPATH_NAMESPACE_NODE           : return String("XPATH_NAMESPACE");
    }
    return String("UNKNOWN");
}

void WebViewCore::dumpNodes(Node* pRootNode)
{
    android_printLog(ANDROID_LOG_DEBUG, "AMT","!!~ --------------- Dumping start --------------- ");
    
    Node* pNode = pRootNode;
    
    while(pNode != NULL)
    {
        Node::NodeType nodeType = pNode->nodeType();
        IntRect nodeRect    = pNode->getRect();
        String  nodeName    = pNode->nodeName();
        String  nodeValue   = pNode->nodeValue();
        nodeValue.replace("\n", "\\n");
        
        String  nodeDumpStr = "\t";

        if(nodeName.isNull() == true)
            nodeName = "NULL";
            
        if(nodeValue.isNull() == true)
            nodeValue = "NULL";

        int parentCount = getParentCount(pNode);
        for(int cnt = 0; cnt<parentCount; cnt++)
        {
            nodeDumpStr += "\t";
        }
        nodeDumpStr += String::number(parentCount) + "\t";

        nodeDumpStr += String("Type(") +getNodeTypeStr(nodeType)+")\t";
        nodeDumpStr += String("Name(") +nodeName +")\t";
        nodeDumpStr += String("Value(")+nodeValue+")\t";
        nodeDumpStr += String("Rect(") +getRectString(nodeRect)+")\t";

        RenderObject* pRenderer = pNode->renderer();
        if(pRenderer != NULL) {
            bool bReplaced = false;
            nodeDumpStr += String("Render name(")       + pRenderer->renderName()+")\t";
            nodeDumpStr += String("Render rect(")       +getRectString(pNode->renderRect(&bReplaced))+")\t";
            nodeDumpStr += String("View rect(")         +getRectString(pRenderer->view()->viewRect())+")\t";
            nodeDumpStr += String("Abs bounding box(")  +getRectString(pRenderer->absoluteBoundingBoxRect(false))+")\t";

            IntRect paintingRootRect;
            pRenderer->paintingRootRect(paintingRootRect);
            nodeDumpStr += String("Painting root rect(")+getRectString(paintingRootRect)+")\t";
        }
        else
        {
            nodeDumpStr += "Render is NULL\t";
        }
        
        android_printLog(ANDROID_LOG_DEBUG, "AMT","!!~ %s\n", nodeDumpStr.utf8().data());

        pNode = pNode->traverseNextNode(pRootNode);
    }    

    android_printLog(ANDROID_LOG_DEBUG, "AMT","!!~ --------------- Dumping end --------------- ");
    
}


static jobject nativeExtractSmartClipData(JNIEnv *env, jobject obj, jint nativeClass, jint x, jint y, jint w, jint h)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);

    String traverseResult = viewImpl->extractSmartClipData(IntRect(x, y, w, h));    
    return wtfStringToJstring(env, traverseResult);
}

// SAMSUNG CHANGES : SmartClip <<
// Copied from CacheBuilder, not sure if this is needed/correct
IntRect getAreaRect(const HTMLAreaElement* area)
{
    Node* node = area->document();
    while ((node = node->traverseNextNode()) != NULL) {
        RenderObject* renderer = node->renderer();
        if (renderer && renderer->isRenderImage()) {
            RenderImage* image = static_cast<RenderImage*>(renderer);
            HTMLMapElement* map = image->imageMap();
            if (map) {
                Node* n;
                for (n = map->firstChild(); n;
                        n = n->traverseNextNode(map)) {
                    if (n == area) {
                        if (area->isDefault())
                            return image->absoluteBoundingBoxRect();
                        return area->computeRect(image);
                    }
                }
            }
        }
    }
    return IntRect();
}

// Copied from CacheBuilder, not sure if this is needed/correct
// TODO: See if this is even needed (I suspect not), and if not remove it
bool validNode(Frame* startFrame, void* matchFrame,
        void* matchNode)
{
    if (matchFrame == startFrame) {
        if (matchNode == NULL)
            return true;
        Node* node = startFrame->document();
        while (node != NULL) {
            if (node == matchNode) {
                const IntRect& rect = node->hasTagName(HTMLNames::areaTag) ?
                    getAreaRect(static_cast<HTMLAreaElement*>(node)) : node->getRect();
                // Consider nodes with empty rects that are not at the origin
                // to be valid, since news.google.com has valid nodes like this
                if (rect.x() == 0 && rect.y() == 0 && rect.isEmpty())
                    return false;
                return true;
            }
            node = node->traverseNextNode();
        }
        return false;
    }
    Frame* child = startFrame->tree()->firstChild();
    while (child) {
        bool result = validNode(child, matchFrame, matchNode);
        if (result)
            return result;
        child = child->tree()->nextSibling();
    }
    return false;
}

static SkTDArray<WebViewCore*> gInstanceList;

void WebViewCore::addInstance(WebViewCore* inst) {
    *gInstanceList.append() = inst;
}

void WebViewCore::removeInstance(WebViewCore* inst) {
    int index = gInstanceList.find(inst);
    ALOG_ASSERT(index >= 0, "RemoveInstance inst not found");
    if (index >= 0) {
        gInstanceList.removeShuffle(index);
    }
}

bool WebViewCore::isInstance(WebViewCore* inst) {
    return gInstanceList.find(inst) >= 0;
}

jobject WebViewCore::getApplicationContext() {

    // check to see if there is a valid webviewcore object
    if (gInstanceList.isEmpty())
        return 0;

    // get the context from the webview
    jobject context = gInstanceList[0]->getContext();

    if (!context)
        return 0;

    // get the application context using JNI
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    jclass contextClass = env->GetObjectClass(context);
    jmethodID appContextMethod = env->GetMethodID(contextClass, "getApplicationContext", "()Landroid/content/Context;");
    env->DeleteLocalRef(contextClass);
    jobject result = env->CallObjectMethod(context, appContextMethod);
    checkException(env);
    return result;
}


struct WebViewCoreStaticMethods {
    jmethodID    m_isSupportedMediaMimeType;
} gWebViewCoreStaticMethods;

// Check whether a media mimeType is supported in Android media framework.
bool WebViewCore::isSupportedMediaMimeType(const WTF::String& mimeType) {
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    jstring jMimeType = wtfStringToJstring(env, mimeType);
    jclass webViewCore = env->FindClass("android/webkit/WebViewCore");
    bool val = env->CallStaticBooleanMethod(webViewCore,
          gWebViewCoreStaticMethods.m_isSupportedMediaMimeType, jMimeType);
    checkException(env);
    env->DeleteLocalRef(webViewCore);
    env->DeleteLocalRef(jMimeType);

    return val;
}

// ----------------------------------------------------------------------------

#define GET_NATIVE_VIEW(env, obj) ((WebViewCore*)env->GetIntField(obj, gWebViewCoreFields.m_nativeClass))
// Field ids for WebViewCore
struct WebViewCoreFields {
    jfieldID    m_nativeClass;
    jfieldID    m_viewportWidth;
    jfieldID    m_viewportHeight;
    jfieldID    m_viewportInitialScale;
    jfieldID    m_viewportMinimumScale;
    jfieldID    m_viewportMaximumScale;
    jfieldID    m_viewportUserScalable;
    jfieldID    m_viewportDensityDpi;
    jfieldID    m_drawIsPaused;
    jfieldID    m_lowMemoryUsageMb;
    jfieldID    m_highMemoryUsageMb;
    jfieldID    m_highUsageDeltaMb;
} gWebViewCoreFields;

// ----------------------------------------------------------------------------

struct WebViewCore::JavaGlue {
    jweak       m_obj;
    jmethodID   m_scrollTo;
    jmethodID   m_contentDraw;
    jmethodID   m_requestListBox;
    jmethodID   m_openFileChooser;
    jmethodID   m_requestSingleListBox;
    jmethodID   m_jsAlert;
    jmethodID   m_jsConfirm;
    jmethodID   m_jsPrompt;
//	SAMSUNG CHANGE >> Print functionality support for JS content		
    jmethodID 	m_printPage;
//	SAMSUNG CHANGE <<
    jmethodID   m_jsUnload;
    jmethodID   m_jsInterrupt;
    jmethodID   m_getWebView;
    jmethodID   m_didFirstLayout;
    jmethodID   m_updateViewport;
    jmethodID   m_sendNotifyProgressFinished;
    jmethodID   m_sendViewInvalidate;
    jmethodID   m_updateTextfield;
    jmethodID   m_updateTextSelection;
//SAMSUNG CHANGES MPSG100006129 >>
    jmethodID   m_updateTextSelectionStartAndEnd;
//SAMSUNG CHANGES MPSG100006129 <<
    jmethodID   m_updateTextSizeAndScroll;
    jmethodID   m_clearTextEntry;
    jmethodID   m_restoreScale;
    jmethodID   m_needTouchEvents;
    jmethodID   m_requestKeyboard;
    jmethodID   m_exceededDatabaseQuota;
    jmethodID   m_reachedMaxAppCacheSize;
    jmethodID   m_populateVisitedLinks;
    jmethodID   m_geolocationPermissionsShowPrompt;
    jmethodID   m_geolocationPermissionsHidePrompt;
    jmethodID   m_getDeviceMotionService;
    jmethodID   m_getDeviceOrientationService;
    jmethodID   m_addMessageToConsole;
    jmethodID   m_focusNodeChanged;
    jmethodID   m_getPluginClass;
    jmethodID   m_showFullScreenPlugin;
    jmethodID   m_hideFullScreenPlugin;
    jmethodID   m_createSurface;
    jmethodID   m_addSurface;
    jmethodID   m_updateSurface;
    jmethodID   m_destroySurface;
    jmethodID   m_getContext;
    jmethodID   m_keepScreenOn;
    jmethodID   m_showRect;
    jmethodID   m_centerFitRect;
    jmethodID   m_setScrollbarModes;
//    jmethodID   m_setInstallableWebApp;//4.2 Merge : removed in 4.2
//    jmethodID   m_enterFullscreenForVideoLayer;//4.2 Merge : removed in 4.2
    jmethodID   m_exitFullscreenVideo;
    jmethodID   m_setWebTextViewAutoFillable;
    jmethodID   m_selectAt;
    jmethodID   m_initEditField;
//SISO_HTMLCOMPOSER start
    jmethodID   m_isEditableSupport;
//SISO_HTMLCOMPOSER end

     jmethodID   m_isDestroyPending;
//SAMSUNG CHANGE Form Navigation >>
    jmethodID   m_initSelectField;
//SAMSUNG CHANGE Form Navigation <<

    jmethodID   m_chromeCanTakeFocus;
    jmethodID   m_chromeTakeFocus;
//SAMSUNG CHANGE HTML5 COLOR <<
    jmethodID   m_openColorChooser;  
//SAMSUNG CHANGE HTML5 COLOR >>
// Samsung Change - HTML5 Web Notification	>>
    #if ENABLE(NOTIFICATIONS)
    jmethodID   m_notificationPermissionsShowPrompt;
    jmethodID   m_notificationManagershow;
    jmethodID   m_notificationManagerCancel;
    jmethodID m_notificationPermissionsHidePrompt;
    #endif
// Samsung Change - HTML5 Web Notification	<<
//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES <<
	jmethodID   m_requestDateTimePickers;
//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES >>
//SAMSUNG CHANGES: MPSG100006003 >>
    jmethodID   m_sendScrollRectOnScreen;
//SAMSUNG CHANGES <<
//SAMSUNG changes <S-PEN Text Selection>
    jmethodID   m_sendStartActionMode;
//SAMSUNG changes <S-PEN Text Selection>

    AutoJObject object(JNIEnv* env) {
        // We hold a weak reference to the Java WebViewCore to avoid memeory
        // leaks due to circular references when WebView.destroy() is not
        // called manually. The WebView and hence the WebViewCore could become
        // weakly reachable at any time, after which the GC could null our weak
        // reference, so we have to check the return value of this method at
        // every use. Note that our weak reference will be nulled before the
        // WebViewCore is finalized.
        return getRealObject(env, m_obj);
    }
};

struct WebViewCore::TextFieldInitDataGlue {
    jmethodID  m_constructor;
    jfieldID   m_fieldPointer;
    jfieldID   m_text;
    jfieldID   m_type;
    jfieldID   m_isSpellCheckEnabled;
    jfieldID   m_isTextFieldNext;
    jfieldID   m_isTextFieldPrev;
//SAMSUNG CHANGE Form Navigation >>
    jfieldID   m_isSelectFieldNext;
    jfieldID   m_isSelectFieldPrev;
//SAMSUNG CHANGE Form Navigation <<
    jfieldID   m_isAutoCompleteEnabled;
    jfieldID   m_name;
    jfieldID   m_label;
    jfieldID   m_maxLength;
    jfieldID   m_contentBounds;
    jfieldID   m_nodeLayerId;
    jfieldID   m_clientRect;//4.2 Merge : m_contentRect changed to m_clientRect
};

/*
 * WebViewCore Implementation
 */

static jmethodID GetJMethod(JNIEnv* env, jclass clazz, const char name[], const char signature[])
{
    jmethodID m = env->GetMethodID(clazz, name, signature);
    ALOG_ASSERT(m, "Could not find method %s", name);
    return m;
}

WebViewCore::WebViewCore(JNIEnv* env, jobject javaWebViewCore, WebCore::Frame* mainframe)
    : m_touchGeneration(0)
    , m_lastGeneration(0)
    , m_javaGlue(new JavaGlue)
    , m_textFieldInitDataGlue(new TextFieldInitDataGlue)
    , m_mainFrame(mainframe)
    , m_popupReply(0)
    , m_blockTextfieldUpdates(false)
//    , m_focusBoundsChanged(false)//4.2 Removed in 4.2
    , m_skipContentDraw(false)
    , m_textGeneration(0)
    , m_maxXScroll(320/4)
    , m_maxYScroll(240/4)
    , m_scrollOffsetX(0)
    , m_scrollOffsetY(0)
    , m_mousePos(WebCore::IntPoint(0,0))
    , m_screenWidth(320)
    , m_screenHeight(240)
    , m_textWrapWidth(320)
    , m_scale(1.0f)
    , m_groupForVisitedLinks(0)
//    , m_isPaused(false)//4.2 Removed in 4.2
    , m_cacheMode(0)
    , m_fullscreenVideoMode(false)
    , m_matchCount(0)
    , m_activeMatchIndex(0)
    , m_activeMatch(0)
    , m_pluginInvalTimer(this, &WebViewCore::pluginInvalTimerFired)
    , m_screenOnCounter(0)
    , m_currentNodeDomNavigationAxis(0)
    , m_deviceMotionAndOrientationManager(this)
   , m_geolocationManager(this)////4.2 Merge : commented for compilation error
#if ENABLE(TOUCH_EVENTS)
    , m_forwardingTouchEvents(false)
#endif
    , m_webRequestContext(0)
    , m_prerenderEnabled(false)
//SAMSUNG CHANGE HTML5 COLOR <<
   , m_colorChooser(0)
//SAMSUNG CHANGE HTML5 COLOR >>
//+HTMLCOMPOSER
   , m_VisibleSelection(VisibleSelection())
//-HTMLCOMPOSER
{
    ALOG_ASSERT(m_mainFrame, "Uh oh, somehow a frameview was made without an initial frame!");

    jclass clazz = env->GetObjectClass(javaWebViewCore);
    m_javaGlue->m_obj = env->NewWeakGlobalRef(javaWebViewCore);
    m_javaGlue->m_scrollTo = GetJMethod(env, clazz, "contentScrollTo", "(IIZZ)V");
    m_javaGlue->m_contentDraw = GetJMethod(env, clazz, "contentDraw", "()V");
    m_javaGlue->m_requestListBox = GetJMethod(env, clazz, "requestListBox", "([Ljava/lang/String;[I[I)V");
    m_javaGlue->m_openFileChooser = GetJMethod(env, clazz, "openFileChooser", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
//SISO_HTMLCOMPOSER start
    m_javaGlue->m_isEditableSupport = GetJMethod(env, clazz, "isEditableSupport", "()Z");
//SISO_HTMLCOMPOSER end
    m_javaGlue->m_isDestroyPending = GetJMethod(env, clazz, "isDestroyPending", "()Z");
    m_javaGlue->m_requestSingleListBox = GetJMethod(env, clazz, "requestListBox", "([Ljava/lang/String;[II)V");
    m_javaGlue->m_jsAlert = GetJMethod(env, clazz, "jsAlert", "(Ljava/lang/String;Ljava/lang/String;)V");
    m_javaGlue->m_jsConfirm = GetJMethod(env, clazz, "jsConfirm", "(Ljava/lang/String;Ljava/lang/String;)Z");
    m_javaGlue->m_jsPrompt = GetJMethod(env, clazz, "jsPrompt", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
//	SAMSUNG CHANGE >> Print functionality support for JS content		
    m_javaGlue->m_printPage = GetJMethod(env, clazz, "printPage", "()V");
//	SAMSUNG CHANGE >> Print functionality support for JS content		
    m_javaGlue->m_jsUnload = GetJMethod(env, clazz, "jsUnload", "(Ljava/lang/String;Ljava/lang/String;)Z");
    m_javaGlue->m_jsInterrupt = GetJMethod(env, clazz, "jsInterrupt", "()Z");
    m_javaGlue->m_getWebView = GetJMethod(env, clazz, "getWebView", "()Landroid/webkit/WebView;");
    m_javaGlue->m_didFirstLayout = GetJMethod(env, clazz, "didFirstLayout", "(Z)V");
    m_javaGlue->m_updateViewport = GetJMethod(env, clazz, "updateViewport", "()V");
    m_javaGlue->m_sendNotifyProgressFinished = GetJMethod(env, clazz, "sendNotifyProgressFinished", "()V");
    m_javaGlue->m_sendViewInvalidate = GetJMethod(env, clazz, "sendViewInvalidate", "(IIII)V");
    m_javaGlue->m_updateTextfield = GetJMethod(env, clazz, "updateTextfield", "(ILjava/lang/String;I)V");//4.2 Merge 
    m_javaGlue->m_updateTextSelection = GetJMethod(env, clazz, "updateTextSelection", "(IIIII)V");
//SAMSUNG CHANGES MPSG100006129 >>
    m_javaGlue->m_updateTextSelectionStartAndEnd = GetJMethod(env, clazz, "updateTextSelectionStartAndEnd", "(IIIIILjava/lang/String;)V");
//SAMSUNG CHANGES MPSG100006129 <<
    m_javaGlue->m_updateTextSizeAndScroll = GetJMethod(env, clazz, "updateTextSizeAndScroll", "(IIIII)V");
    m_javaGlue->m_clearTextEntry = GetJMethod(env, clazz, "clearTextEntry", "()V");
    m_javaGlue->m_restoreScale = GetJMethod(env, clazz, "restoreScale", "(FF)V");
    m_javaGlue->m_needTouchEvents = GetJMethod(env, clazz, "needTouchEvents", "(Z)V");
    m_javaGlue->m_requestKeyboard = GetJMethod(env, clazz, "requestKeyboard", "(Z)V");
    m_javaGlue->m_exceededDatabaseQuota = GetJMethod(env, clazz, "exceededDatabaseQuota", "(Ljava/lang/String;Ljava/lang/String;JJ)V");
    m_javaGlue->m_reachedMaxAppCacheSize = GetJMethod(env, clazz, "reachedMaxAppCacheSize", "(JJ)V");//4.2 Merge
    m_javaGlue->m_populateVisitedLinks = GetJMethod(env, clazz, "populateVisitedLinks", "()V");
    m_javaGlue->m_geolocationPermissionsShowPrompt = GetJMethod(env, clazz, "geolocationPermissionsShowPrompt", "(Ljava/lang/String;)V");
    m_javaGlue->m_geolocationPermissionsHidePrompt = GetJMethod(env, clazz, "geolocationPermissionsHidePrompt", "()V");
    m_javaGlue->m_getDeviceMotionService = GetJMethod(env, clazz, "getDeviceMotionService", "()Landroid/webkit/DeviceMotionService;");
    m_javaGlue->m_getDeviceOrientationService = GetJMethod(env, clazz, "getDeviceOrientationService", "()Landroid/webkit/DeviceOrientationService;");
    m_javaGlue->m_addMessageToConsole = GetJMethod(env, clazz, "addMessageToConsole", "(Ljava/lang/String;ILjava/lang/String;I)V");
    m_javaGlue->m_focusNodeChanged = GetJMethod(env, clazz, "focusNodeChanged", "(ILandroid/webkit/WebViewCore$WebKitHitTest;)V");
    m_javaGlue->m_getPluginClass = GetJMethod(env, clazz, "getPluginClass", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Class;");
    m_javaGlue->m_showFullScreenPlugin = GetJMethod(env, clazz, "showFullScreenPlugin", "(Landroid/webkit/ViewManager$ChildView;II)V");
    m_javaGlue->m_hideFullScreenPlugin = GetJMethod(env, clazz, "hideFullScreenPlugin", "()V");
    m_javaGlue->m_createSurface = GetJMethod(env, clazz, "createSurface", "(Landroid/view/View;)Landroid/webkit/ViewManager$ChildView;");
    m_javaGlue->m_addSurface = GetJMethod(env, clazz, "addSurface", "(Landroid/view/View;IIII)Landroid/webkit/ViewManager$ChildView;");
    m_javaGlue->m_updateSurface = GetJMethod(env, clazz, "updateSurface", "(Landroid/webkit/ViewManager$ChildView;IIII)V");
    m_javaGlue->m_destroySurface = GetJMethod(env, clazz, "destroySurface", "(Landroid/webkit/ViewManager$ChildView;)V");
    m_javaGlue->m_getContext = GetJMethod(env, clazz, "getContext", "()Landroid/content/Context;");
    m_javaGlue->m_keepScreenOn = GetJMethod(env, clazz, "keepScreenOn", "(Z)V");
    m_javaGlue->m_showRect = GetJMethod(env, clazz, "showRect", "(IIIIIIFFFF)V");
    m_javaGlue->m_centerFitRect = GetJMethod(env, clazz, "centerFitRect", "(IIII)V");
    m_javaGlue->m_setScrollbarModes = GetJMethod(env, clazz, "setScrollbarModes", "(II)V");
//    m_javaGlue->m_setInstallableWebApp = GetJMethod(env, clazz, "setInstallableWebApp", "()V");//4.2 Merge : removed in 4.2
#if ENABLE(VIDEO)
//    m_javaGlue->m_enterFullscreenForVideoLayer = GetJMethod(env, clazz, "enterFullscreenForVideoLayer", "(ILjava/lang/String;)V");//4.2 Merge : removed in 4.2
    m_javaGlue->m_exitFullscreenVideo = GetJMethod(env, clazz, "exitFullscreenVideo", "()V");
#endif
    m_javaGlue->m_setWebTextViewAutoFillable = GetJMethod(env, clazz, "setWebTextViewAutoFillable", "(ILjava/lang/String;)V");
    m_javaGlue->m_selectAt = GetJMethod(env, clazz, "selectAt", "(II)V");
    m_javaGlue->m_initEditField = GetJMethod(env, clazz, "initEditField", "(IIILandroid/webkit/WebViewCore$TextFieldInitData;)V");
//SAMSUNG CHANGE Form Navigation >>
    m_javaGlue->m_initSelectField = GetJMethod(env, clazz, "initSelectField", "(Landroid/webkit/WebViewCore$TextFieldInitData;)V");
//SAMSUNG CHANGE Form Navigation <<
    m_javaGlue->m_chromeCanTakeFocus = GetJMethod(env, clazz, "chromeCanTakeFocus", "(I)Z");
    m_javaGlue->m_chromeTakeFocus = GetJMethod(env, clazz, "chromeTakeFocus", "(I)V");
//SAMSUNG CHANGE HTML5 COLOR <<
    m_javaGlue->m_openColorChooser = GetJMethod(env, clazz, "openColorChooser", "()V");
//SAMSUNG CHANGE HTML5 COLOR >>

    // Samsung Change - HTML5 Web Notification	>>
    #if ENABLE(NOTIFICATIONS)
    m_javaGlue->m_notificationPermissionsShowPrompt = GetJMethod(env, clazz, "notificationPermissionsShowPrompt", "(Ljava/lang/String;)V");
    m_javaGlue->m_notificationManagershow = GetJMethod(env, clazz, "notificationManagershow", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
    m_javaGlue->m_notificationManagerCancel = GetJMethod(env, clazz, "notificationManagerCancel", "(I)V");
     m_javaGlue->m_notificationPermissionsHidePrompt = GetJMethod(env, clazz, "notificationPermissionsHidePrompt", "()V");
    #endif
    // Samsung Change - HTML5 Web Notification	<<

//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES <<
    m_javaGlue->m_requestDateTimePickers = GetJMethod(env, clazz, "requestDateTimePickers", "(Ljava/lang/String;Ljava/lang/String;)V");	
//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES >>	
//SAMSUNG CHANGES: MPSG100006003 >>
    m_javaGlue->m_sendScrollRectOnScreen = GetJMethod(env, clazz, "sendScrollRectOnScreen", "(IIII)V");	
//SAMSUNG CHANGES <<
//SAMSUNG changes <S-PEN Text Selection>
    m_javaGlue->m_sendStartActionMode = GetJMethod(env, clazz, "sendStartActionMode", "(Z)V");	
//SAMSUNG changes <S-PEN Text Selection>
    env->DeleteLocalRef(clazz);

    env->SetIntField(javaWebViewCore, gWebViewCoreFields.m_nativeClass, (jint)this);

    jclass tfidClazz = env->FindClass("android/webkit/WebViewCore$TextFieldInitData");
    m_textFieldInitDataGlue->m_fieldPointer = env->GetFieldID(tfidClazz, "mFieldPointer", "I");
    m_textFieldInitDataGlue->m_text = env->GetFieldID(tfidClazz, "mText", "Ljava/lang/String;");
    m_textFieldInitDataGlue->m_type = env->GetFieldID(tfidClazz, "mType", "I");
    m_textFieldInitDataGlue->m_isSpellCheckEnabled = env->GetFieldID(tfidClazz, "mIsSpellCheckEnabled", "Z");
    m_textFieldInitDataGlue->m_isTextFieldNext = env->GetFieldID(tfidClazz, "mIsTextFieldNext", "Z");
    m_textFieldInitDataGlue->m_isTextFieldPrev = env->GetFieldID(tfidClazz, "mIsTextFieldPrev", "Z");
//SAMSUNG CHANGE Form Navigation >>
    m_textFieldInitDataGlue->m_isSelectFieldNext = env->GetFieldID(tfidClazz, "mIsSelectFieldNext", "Z");
    m_textFieldInitDataGlue->m_isSelectFieldPrev = env->GetFieldID(tfidClazz, "mIsSelectFieldPrev", "Z");
//SAMSUNG CHANGE Form Navigation <<
    m_textFieldInitDataGlue->m_isAutoCompleteEnabled = env->GetFieldID(tfidClazz, "mIsAutoCompleteEnabled", "Z");
    m_textFieldInitDataGlue->m_name = env->GetFieldID(tfidClazz, "mName", "Ljava/lang/String;");
    m_textFieldInitDataGlue->m_label = env->GetFieldID(tfidClazz, "mLabel", "Ljava/lang/String;");
    m_textFieldInitDataGlue->m_maxLength = env->GetFieldID(tfidClazz, "mMaxLength", "I");
    m_textFieldInitDataGlue->m_contentBounds = env->GetFieldID(tfidClazz, "mContentBounds", "Landroid/graphics/Rect;");
    m_textFieldInitDataGlue->m_nodeLayerId = env->GetFieldID(tfidClazz, "mNodeLayerId", "I");
    m_textFieldInitDataGlue->m_clientRect = env->GetFieldID(tfidClazz, "mClientRect", "Landroid/graphics/Rect;");//4.2 Merge
    m_textFieldInitDataGlue->m_constructor = GetJMethod(env, tfidClazz, "<init>", "()V");
    env->DeleteLocalRef(tfidClazz);

    PageGroup::setShouldTrackVisitedLinks(true);

    clearContent();

    MemoryUsage::setLowMemoryUsageMb(env->GetIntField(javaWebViewCore, gWebViewCoreFields.m_lowMemoryUsageMb));
    MemoryUsage::setHighMemoryUsageMb(env->GetIntField(javaWebViewCore, gWebViewCoreFields.m_highMemoryUsageMb));
    MemoryUsage::setHighUsageDeltaMb(env->GetIntField(javaWebViewCore, gWebViewCoreFields.m_highUsageDeltaMb));

    WebViewCore::addInstance(this);

//SISO_HTMLCOMPOSER start
    m_composingVisibleSelection = VisibleSelection();
    m_underLineVisibleSelection = VisibleSelection();
    m_imStr = 0;
    m_imEnd = 0;
//SISO_HTMLCOMPOSER end
    AndroidNetworkLibraryImpl::InitWithApplicationContext(env, 0);

    // increase the font cache size beyond the standard system setting
    SkGraphics::SetFontCacheLimit(1572864); // 1572864 bytes == 1.5 MB

    // Static initialisation of certain important V8 static data gets performed at system startup when
    // libwebcore gets loaded. We now need to associate the WebCore thread with V8 to complete
    // initialisation.
    v8::V8::Initialize();

//SAMSUNG_WEB_WORKER_CHANGES >>
    WebCore::V8BindingPerIsolateData::ensureInitialized(v8::Isolate::GetCurrent());
//SAMSUNG_WEB_WORKER_CHANGES <<


    // Configure any RuntimeEnabled features that we need to change from their default now.
    // See WebCore/bindings/generic/RuntimeEnabledFeatures.h

    // HTML5 History API
    RuntimeEnabledFeatures::setPushStateEnabled(true);
    if (m_mainFrame)
        m_mainFrame->settings()->setMinDOMTimerInterval(FOREGROUND_TIMER_INTERVAL);
}

WebViewCore::~WebViewCore()
{
    WebViewCore::removeInstance(this);

    // Release the focused view
    Release(m_popupReply);

//SAMSUNG CHANGE HTML5 COLOR <<	
    if(m_colorChooser){
        delete(m_colorChooser);   
	m_colorChooser = 0; 	
    }
//SAMSUNG CHANGE HTML5 COLOR >>

    if (m_javaGlue->m_obj) {
        JNIEnv* env = JSC::Bindings::getJNIEnv();
        env->DeleteWeakGlobalRef(m_javaGlue->m_obj);
        m_javaGlue->m_obj = 0;
    }
    delete m_javaGlue;
}

WebViewCore* WebViewCore::getWebViewCore(const WebCore::FrameView* view)
{
    if (!view)
        return 0;
    if (view->platformWidget())
        return static_cast<WebFrameView*>(view->platformWidget())->webViewCore();
    Frame* frame = view->frame();
    while (Frame* parent = frame->tree()->parent())
        frame = parent;
    WebFrameView* webFrameView = 0;
    if (frame && frame->view())
        webFrameView = static_cast<WebFrameView*>(frame->view()->platformWidget());
    if (!webFrameView)
        return 0;
    return webFrameView->webViewCore();
}

WebViewCore* WebViewCore::getWebViewCore(const WebCore::ScrollView* view)
{
    if (!view)
        return 0;
    if (view->platformWidget())
        return static_cast<WebFrameView*>(view->platformWidget())->webViewCore();
    const FrameView* frameView = 0;
    if (view->isFrameView())
        frameView = static_cast<const FrameView*>(view);
    else {
        frameView = static_cast<const FrameView*>(view->root());
        if (!frameView)
            return 0;
    }
    return getWebViewCore(frameView);
}

static bool layoutIfNeededRecursive(WebCore::Frame* f)
{
    if (!f)
        return true;

    WebCore::FrameView* v = f->view();
    if (!v)
        return true;
    v->updateLayoutAndStyleIfNeededRecursive();
    return !v->needsLayout();
}

WebCore::Node* WebViewCore::currentFocus()
{
    return focusedFrame()->document()->focusedNode();
}

void WebViewCore::layout()
{
    TRACE_METHOD();

    // if there is no document yet, just return
    if (!m_mainFrame->document()) {
        ALOGV("!m_mainFrame->document()");
        return;
    }

    // Call layout to ensure that the contentWidth and contentHeight are correct
    // it's fine for layout to gather invalidates, but defeat sending a message
    // back to java to call webkitDraw, since we're already in the middle of
    // doing that
    bool success = layoutIfNeededRecursive(m_mainFrame);

    // We may be mid-layout and thus cannot draw.
    if (!success)
        return;

    // if the webkit page dimensions changed, discard the pictureset and redraw.
    WebCore::FrameView* view = m_mainFrame->view();
    int width = view->contentsWidth();
    int height = view->contentsHeight();

    // Use the contents width and height as a starting point.
    SkIRect contentRect;
    contentRect.set(0, 0, width, height);
    SkIRect total(contentRect);

    // Traverse all the frames and add their sizes if they are in the visible
    // rectangle.
    for (WebCore::Frame* frame = m_mainFrame->tree()->traverseNext(); frame;
            frame = frame->tree()->traverseNext()) {
        // If the frame doesn't have an owner then it is the top frame and the
        // view size is the frame size.
        WebCore::RenderPart* owner = frame->ownerRenderer();
        if (owner && owner->style()->visibility() == VISIBLE) {
            int x = owner->x();
            int y = owner->y();

            // Traverse the tree up to the parent to find the absolute position
            // of this frame.
            WebCore::Frame* parent = frame->tree()->parent();
            while (parent) {
                WebCore::RenderPart* parentOwner = parent->ownerRenderer();
                if (parentOwner) {
                    x += parentOwner->x();
                    y += parentOwner->y();
                }
                parent = parent->tree()->parent();
            }
            // Use the owner dimensions so that padding and border are
            // included.
            int right = x + owner->width();
            int bottom = y + owner->height();
            SkIRect frameRect = {x, y, right, bottom};
            // Ignore a width or height that is smaller than 1. Some iframes
            // have small dimensions in order to be hidden. The iframe
            // expansion code does not expand in that case so we should ignore
            // them here.
            if (frameRect.width() > 1 && frameRect.height() > 1
                    && SkIRect::Intersects(total, frameRect))
                total.join(x, y, right, bottom);
        }
    }

    // If the new total is larger than the content, resize the view to include
    // all the content.
    if (!contentRect.contains(total)) {
        // TODO: Does this ever happen? Is this needed now that we don't flatten
        // frames?
        // Resize the view to change the overflow clip.
        view->resize(total.fRight, total.fBottom);

        // We have to force a layout in order for the clip to change.
        m_mainFrame->contentRenderer()->setNeedsLayoutAndPrefWidthsRecalc();
        view->forceLayout();

        // Relayout similar to above
        layoutIfNeededRecursive(m_mainFrame);
    }
}

void WebViewCore::recordPicturePile()
{
    // if the webkit page dimensions changed, discard the pictureset and redraw.
    WebCore::FrameView* view = m_mainFrame->view();
    int width = view ? view->contentsWidth() : 0;
    int height = view ? view->contentsHeight() : 0;

    m_content.setSize(IntSize(width, height));
    if (m_mainFrame->settings() != NULL && m_mainFrame->settings()->isSkiaPictureEnabled()) {
		m_content.setSkiaPictureEnabled(true);
    }
    // Rebuild the pictureset (webkit repaint)
    m_content.updatePicturesIfNeeded(this);
}

void WebViewCore::clearContent()
{
    m_content.reset();
    updateLocale();
}

// 4.2 Merge BEGIN <<
//Following code Removed in 4.2
//bool WebViewCore::focusBoundsChanged()
//{
//    bool result = m_focusBoundsChanged;
//    m_focusBoundsChanged = false;
//    return result;
//}
// 4.2 Merge END >>
void WebViewCore::paintContents(WebCore::GraphicsContext* gc, WebCore::IntRect& dirty)
{
    WebCore::FrameView* view = m_mainFrame->view();
    if (!view) {
        gc->setFillColor(WebCore::Color::white, WebCore::ColorSpaceDeviceRGB);
        gc->fillColor();
        return;
    }

    IntPoint origin = view->minimumScrollPosition();
    IntRect drawArea = dirty;
    gc->translate(-origin.x(), -origin.y());
    drawArea.move(origin.x(), origin.y());
    view->platformWidget()->draw(gc, drawArea);
}

void WebViewCore::setPrerenderingEnabled(bool enable)
{
    if (m_prerenderEnabled != enable) {
        MutexLocker locker(m_prerenderLock);
        m_prerenderEnabled = enable;
    }
}

bool WebViewCore::prerenderingEnabled()
{
    MutexLocker locker(m_prerenderLock);
    return m_prerenderEnabled;
}

SkCanvas* WebViewCore::createPrerenderCanvas(PrerenderedInval* prerendered)
{
    // Has WebView disabled prerenders (not attached, etc...)?
    if (!prerenderingEnabled())
        return 0;
    // Does this WebView have focus?
    if (!m_mainFrame->page()->focusController()->isActive())
        return 0;
    // Are we scrolling?
    if (currentTimeMS() - m_scrollSetTime < PRERENDER_AFTER_SCROLL_DELAY)
        return 0;
    // Do we have anything to render?
    if (prerendered->area.isEmpty())
        return 0;
    FloatRect scaleTemp(m_scrollOffsetX, m_scrollOffsetY, m_screenWidth, m_screenHeight);
    scaleTemp.scale(m_scale);
    IntRect visibleTileClip = enclosingIntRect(scaleTemp);
    FloatRect scaledArea = prerendered->area;
    scaledArea.scale(m_scale);
    IntRect enclosingScaledArea = enclosingIntRect(scaledArea);
    if (enclosingScaledArea.isEmpty())
        return 0;
    // "round out" the screen to tile boundaries so that we can clip yet still
    // cover any visible tiles with the prerender
    int tw = TilesManager::tileWidth();
    int th = TilesManager::tileHeight();
    float left = tw * (int) (visibleTileClip.x() / tw);
    float top = th * (int) (visibleTileClip.y() / th);
    float right = tw * (int) ceilf(visibleTileClip.maxX() / (float) tw);
    float bottom = th * (int) ceilf(visibleTileClip.maxY() / (float) th);
    visibleTileClip = IntRect(left, top, right - left, bottom - top);
    enclosingScaledArea.intersect(visibleTileClip);
    if (enclosingScaledArea.isEmpty())
        return 0;
   //Samsung Change>> Can not Allocate more than 2 GB memory .
    if( enclosingScaledArea.height() > MAX_HEIGHT ) 
   {
	return 0;
    }
   //Samsung Change <<
    prerendered->screenArea = enclosingScaledArea;
    FloatRect enclosingDocArea(enclosingScaledArea);
    enclosingDocArea.scale(1 / m_scale);
    prerendered->area = enclosingIntRect(enclosingDocArea);
    if (prerendered->area.isEmpty())
        return 0;
    prerendered->bitmap.setConfig(SkBitmap::kARGB_8888_Config,
                                  enclosingScaledArea.width(),
                                  enclosingScaledArea.height());
    prerendered->bitmap.allocPixels();
    SkCanvas* bitmapCanvas = new SkCanvas(prerendered->bitmap);
    bitmapCanvas->scale(m_scale, m_scale);
    bitmapCanvas->translate(-enclosingDocArea.x(), -enclosingDocArea.y());
    return bitmapCanvas;
}

void WebViewCore::notifyAnimationStarted()
{
    // We notify webkit that the animations have begun
    // TODO: handle case where not all have begun
    ChromeClientAndroid* chromeC = static_cast<ChromeClientAndroid*>(m_mainFrame->page()->chrome()->client());
    GraphicsLayerAndroid* root = static_cast<GraphicsLayerAndroid*>(chromeC->layersSync());
    if (root)
        root->notifyClientAnimationStarted();

}

BaseLayerAndroid* WebViewCore::createBaseLayer(GraphicsLayerAndroid* root)
{
    // We set the background color
    Color background = Color::white;

    bool bodyHasFixedBackgroundImage = false;
    bool bodyHasCSSBackground = false;

    if (m_mainFrame && m_mainFrame->document()
        && m_mainFrame->document()->body()) {

        Document* document = m_mainFrame->document();
        RefPtr<RenderStyle> style = document->styleForElementIgnoringPendingStylesheets(document->body());
        if (style->hasBackground()) {
            background = style->visitedDependentColor(CSSPropertyBackgroundColor);
            bodyHasCSSBackground = true;
        }
        WebCore::FrameView* view = m_mainFrame->view();
        if (view) {
            Color viewBackground = view->baseBackgroundColor();
            background = bodyHasCSSBackground ? viewBackground.blend(background) : viewBackground;
        }
        if (style->hasFixedBackgroundImage()) {
            Image* backgroundImage = FixedBackgroundImageLayerAndroid::GetCachedImage(style);
            if (backgroundImage && backgroundImage->width() > 1 && backgroundImage->height() > 1)
                bodyHasFixedBackgroundImage = true;
        }
    }

    PicturePileLayerContent* content = new PicturePileLayerContent(m_content);
    m_content.clearPrerenders();

    BaseLayerAndroid* realBase = 0;
    LayerAndroid* base = 0;

    //If we have a fixed background image on the body element, the fixed image
    // will be contained in the PictureSet (the content object), and the foreground
    //of the body element will be moved to a layer.
    //In that case, let's change the hierarchy to obtain:
    //
    //BaseLayerAndroid
    // \- FixedBackgroundBaseLayerAndroid (fixed positioning)
    // \- ForegroundBaseLayerAndroid
    //   \- root layer (webkit composited tree)

    if (bodyHasFixedBackgroundImage) {
        base = new ForegroundBaseLayerAndroid(0);
        base->setSize(content->width(), content->height());

        Document* document = m_mainFrame->document();
        RefPtr<RenderStyle> style = document->styleForElementIgnoringPendingStylesheets(document->body());

        FixedBackgroundImageLayerAndroid* baseBackground =
             new FixedBackgroundImageLayerAndroid(style, content->width(), content->height());

        realBase = new BaseLayerAndroid(0);
        realBase->setSize(content->width(), content->height());
        realBase->addChild(baseBackground);
        realBase->addChild(base);
        baseBackground->unref();
        base->unref();
    } else {
        realBase = new BaseLayerAndroid(content);
        base = realBase;
    }

    realBase->setBackgroundColor(background);

    SkSafeUnref(content);

    // We update the layers
    if (root) {
        LayerAndroid* copyLayer = new LayerAndroid(*root->contentLayer());
        base->addChild(copyLayer);
        copyLayer->unref();
        root->contentLayer()->clearDirtyRegion();
    }

    return realBase;
}

BaseLayerAndroid* WebViewCore::recordContent(SkIPoint* point)
{
    m_skipContentDraw = true;
    layout();
    ChromeClientAndroid* chromeC = static_cast<ChromeClientAndroid*>(m_mainFrame->page()->chrome()->client());
    GraphicsLayerAndroid* root = static_cast<GraphicsLayerAndroid*>(chromeC->layersSync());
    m_skipContentDraw = false;
    recordPicturePile();
    if(isDestroyPending()){
		 ALOGV("Leak: recordContent return NULL, skipping BaseLayer Creation (Destroy Pending)");
		 m_content.reset();
		return NULL;
    	}
    BaseLayerAndroid* baseLayer = createBaseLayer(root);

    baseLayer->markAsDirty(m_content.dirtyRegion());
    m_content.dirtyRegion().setEmpty();
#if USE(ACCELERATED_COMPOSITING)
#else
    baseLayer->markAsDirty(m_rebuildInval);
#endif
    point->fX = m_content.size().width();
    point->fY = m_content.size().height();

    return baseLayer;
}

void WebViewCore::scrollTo(int x, int y, bool animate)
{
    ALOG_ASSERT(m_javaGlue->m_obj, "A Java widget was not associated with this view bridge!");

    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_scrollTo,
            x, y, animate, false);
    checkException(env);
}

//SAMSUNG CHANGES: MPSG100006003 >>
void WebViewCore::scrollRectOnScreen(IntRect rect)
{
    ALOG_ASSERT(m_javaGlue->m_obj, "A Java widget was not associated with this view bridge!");

    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    ALOGV("scrollRectOnScreen() rect=[%d, %d, w=%d h=%d]", rect.x(),rect.y(), rect.width(), rect.height());

    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_sendScrollRectOnScreen, rect.x(), rect.y(), rect.maxX(), rect.maxY());
    checkException(env);
}
//SAMSUNG CHANGES <<

//SAMSUNG changes <S-PEN Text Selection>
void WebViewCore::startActionMode(bool textSelected)
{
    ALOG_ASSERT(m_javaGlue->m_obj, "A Java widget was not associated with this view bridge!");

    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;

    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_sendStartActionMode, textSelected);
    checkException(env);
}
//SAMSUNG changes <S-PEN Text Selection>
void WebViewCore::sendNotifyProgressFinished()
{
    ALOG_ASSERT(m_javaGlue->m_obj, "A Java widget was not associated with this view bridge!");
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_sendNotifyProgressFinished);
    checkException(env);
}

void WebViewCore::viewInvalidate(const WebCore::IntRect& rect)
{
    ALOG_ASSERT(m_javaGlue->m_obj, "A Java widget was not associated with this view bridge!");
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(),
                        m_javaGlue->m_sendViewInvalidate,
                        rect.x(), rect.y(), rect.maxX(), rect.maxY());
    checkException(env);
}

void WebViewCore::contentDraw()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_contentDraw);
    checkException(env);
}

void WebViewCore::contentInvalidate(const WebCore::IntRect &r)
{
    if((NULL==m_mainFrame)  || (NULL== m_mainFrame->view())) // Preventive check for P120930-0624
	   return ;
    IntPoint origin = m_mainFrame->view()->minimumScrollPosition();
    IntRect dirty = r;
    dirty.move(-origin.x(), -origin.y());
    m_content.invalidate(dirty);
    if (!m_skipContentDraw)
        contentDraw();
}

//SAMSUNG CHANGES: mobile page zoom scale change issue - merge from ICS >>
void WebViewCore::recalcWidthAndForceLayout()
{
    if(!m_mainFrame->document()){
        ALOGV("!m_mainFrame->document()");
        return;
    }

    WebCore::FrameView* view = m_mainFrame->view();
    m_mainFrame->contentRenderer()->setNeedsLayoutAndPrefWidthsRecalc();
    view->forceLayout();
}
//SAMSUNG CHANGES <<

void WebViewCore::contentInvalidateAll()
{
    WebCore::FrameView* view = m_mainFrame->view();
    contentInvalidate(WebCore::IntRect(0, 0,
        view->contentsWidth(), view->contentsHeight()));
}

void WebViewCore::offInvalidate(const WebCore::IntRect &r)
{
    // FIXME: these invalidates are offscreen, and can be throttled or
    // deferred until the area is visible. For now, treat them as
    // regular invals so that drawing happens (inefficiently) for now.
    contentInvalidate(r);
}

void WebViewCore::didFirstLayout()
{
    ALOG_ASSERT(m_javaGlue->m_obj, "A Java widget was not associated with this view bridge!");

    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;

    const WebCore::KURL& url = m_mainFrame->document()->url();
    if (url.isEmpty())
        return;
    ALOGV("::WebCore:: didFirstLayout %s", url.string().ascii().data());

    WebCore::FrameLoadType loadType = m_mainFrame->loader()->loadType();

    // SAMSUNG CHANGE + MPSG100005448 - for mysingle app when we scroll and load same url with more data scroll position is not retained.
    if (m_mainFrame->settings() == NULL || m_mainFrame->settings()->isBrowserApp()) {
        env->CallVoidMethod(javaObject.get(), m_javaGlue->m_didFirstLayout,
            loadType == WebCore::FrameLoadTypeStandard
            // When redirect with locked history, we would like to reset the
            // scale factor. This is important for www.yahoo.com as it is
            // redirected to www.yahoo.com/?rs=1 on load.
            || loadType == WebCore::FrameLoadTypeRedirectWithLockedBackForwardList
            // When "request desktop page" is used, we want to treat it as
            // a newly-loaded page.
            || loadType == WebCore::FrameLoadTypeSame);
    } else {
        env->CallVoidMethod(javaObject.get(), m_javaGlue->m_didFirstLayout,
            loadType == WebCore::FrameLoadTypeStandard
            // When redirect with locked history, we would like to reset the
            // scale factor. This is important for www.yahoo.com as it is
            // redirected to www.yahoo.com/?rs=1 on load.
            || loadType == WebCore::FrameLoadTypeRedirectWithLockedBackForwardList);
    }
    // SAMSUNG CHANGE -
    checkException(env);
}

void WebViewCore::updateViewport()
{
    ALOG_ASSERT(m_javaGlue->m_obj, "A Java widget was not associated with this view bridge!");

    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_updateViewport);
    checkException(env);
}

void WebViewCore::restoreScale(float scale, float textWrapScale)
{
    ALOG_ASSERT(m_javaGlue->m_obj, "A Java widget was not associated with this view bridge!");

    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_restoreScale, scale, textWrapScale);
    checkException(env);
}

void WebViewCore::needTouchEvents(bool need)
{
    ALOG_ASSERT(m_javaGlue->m_obj, "A Java widget was not associated with this view bridge!");

#if ENABLE(TOUCH_EVENTS)
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;

    if (m_forwardingTouchEvents == need)
        return;

    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_needTouchEvents, need);
    checkException(env);

    m_forwardingTouchEvents = need;
#endif
}

void WebViewCore::requestKeyboard(bool showKeyboard)
{
    ALOG_ASSERT(m_javaGlue->m_obj, "A Java widget was not associated with this view bridge!");

    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_requestKeyboard, showKeyboard);
    checkException(env);
}

void WebViewCore::notifyProgressFinished()
{
    sendNotifyProgressFinished();
}

void WebViewCore::setScrollOffset(bool sendScrollEvent, int dx, int dy)
{
    if (m_scrollOffsetX != dx || m_scrollOffsetY != dy) {
        m_scrollOffsetX = dx;
        m_scrollOffsetY = dy;
        m_scrollSetTime = currentTimeMS();
        // The visible rect is located within our coordinate space so it
        // contains the actual scroll position. Setting the location makes hit
        // testing work correctly.
        m_mainFrame->view()->platformWidget()->setLocation(m_scrollOffsetX,
                m_scrollOffsetY);
        if (sendScrollEvent) {
            m_mainFrame->eventHandler()->sendScrollEvent();

            // Only update history position if it's user scrolled.
            // Update history item to reflect the new scroll position.
            // This also helps save the history information when the browser goes to
            // background, so scroll position will be restored if browser gets
            // killed while in background.
            WebCore::HistoryController* history = m_mainFrame->loader()->history();
            // Because the history item saving could be heavy for large sites and
            // scrolling can generate lots of small scroll offset, the following code
            // reduces the saving frequency.
            static const int MIN_SCROLL_DIFF = 32;
            if (history->currentItem()) {
                WebCore::IntPoint currentPoint = history->currentItem()->scrollPoint();
                if (std::abs(currentPoint.x() - dx) >= MIN_SCROLL_DIFF ||
                    std::abs(currentPoint.y() - dy) >= MIN_SCROLL_DIFF) {
                    history->saveScrollPositionAndViewStateToItem(history->currentItem());
                }
            }
        }

        // update the currently visible screen
        sendPluginVisibleScreen();
    }
}

void WebViewCore::setGlobalBounds(int x, int y, int h, int v)
{
    m_mainFrame->view()->platformWidget()->setWindowBounds(x, y, h, v);
}

void WebViewCore::setSizeScreenWidthAndScale(int width, int height,
    int textWrapWidth, float scale, int screenWidth, int screenHeight,
    int anchorX, int anchorY, bool ignoreHeight)
{
    // Ignore the initial empty document.
    const WebCore::KURL& url = m_mainFrame->document()->url();
    if (url.isEmpty())
        return;

    WebCoreViewBridge* window = m_mainFrame->view()->platformWidget();
    int ow = window->width();
    int oh = window->height();
    int osw = m_screenWidth;
    int osh = m_screenHeight;
    int otw = m_textWrapWidth;
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_textWrapWidth = textWrapWidth;
    if (scale >= 0) // negative means keep the current scale
        m_scale = scale;
    m_maxXScroll = screenWidth >> 2;
    m_maxYScroll = m_maxXScroll * height / width;
    // Don't reflow if the diff is small.
    const bool reflow = otw && textWrapWidth &&
        ((float) abs(otw - textWrapWidth) / textWrapWidth) >= 0.01f;

    // When the screen size change, fixed positioned element should be updated.
    // This is supposed to be light weighted operation without a full layout.
    if (osh != screenHeight || osw != screenWidth)
        m_mainFrame->view()->updatePositionedObjects();

    if (ow != width || (!ignoreHeight && oh != height) || reflow) {
        WebCore::RenderObject *r = m_mainFrame->contentRenderer();
        if (r) {
            WebCore::IntPoint anchorPoint = WebCore::IntPoint(anchorX, anchorY);
            RefPtr<WebCore::Node> node;
            WebCore::IntRect bounds;
            WebCore::IntPoint offset;
            // If the text wrap changed, it is probably zoom change or
            // orientation change. Try to keep the anchor at the same place.
            if (otw && textWrapWidth && otw != textWrapWidth &&
                (anchorX != 0 || anchorY != 0)) {
                WebCore::HitTestResult hitTestResult =
                        m_mainFrame->eventHandler()->hitTestResultAtPoint(
                                anchorPoint, false);
                node = hitTestResult.innerNode();
                if (node && !node->isTextNode()) {
                    // If the hitTestResultAtPoint didn't find a suitable node
                    // for anchoring, try again with some slop.
                    static const int HIT_SLOP = 30;
                    anchorPoint.move(HIT_SLOP, HIT_SLOP);
                    hitTestResult =
                        m_mainFrame->eventHandler()->hitTestResultAtPoint(
                                anchorPoint, false);
                    node = hitTestResult.innerNode();
                }
				
				// SAMSUNG CHANGES ++ :
				// if node is fullscreen size node, it returns 0,0. so viewpoint goes to 0,0.
				// eg. naver pc page, double tap blank space of top-right side and rotate > viewpoint goes to 0,0
				if(node) {
					bounds = node->getRect();
					if ( bounds.width() == ow ) node = NULL ;
				}
				// SAMSUNG CHANGES --
            }
            if (node) {
                bounds = node->getRect();
                // sites like nytimes.com insert a non-standard tag <nyt_text>
                // in the html. If it is the HitTestResult, it may have zero
                // width and height. In this case, use its parent node.
                if (bounds.width() == 0) {
                    node = node->parentOrHostNode();
                    if (node) {
                        bounds = node->getRect();
                    }
                }
            }

            // Set the size after finding the old anchor point as
            // hitTestResultAtPoint causes a layout.
            window->setSize(width, height);
            window->setVisibleSize(screenWidth, screenHeight);
            if (width != screenWidth) {
                m_mainFrame->view()->setUseFixedLayout(true);
                m_mainFrame->view()->setFixedLayoutSize(IntSize(width, height));
            } else
                m_mainFrame->view()->setUseFixedLayout(false);
            r->setNeedsLayoutAndPrefWidthsRecalc();
            if (m_mainFrame->view()->didFirstLayout())
                m_mainFrame->view()->forceLayout();

            // scroll to restore current screen center
            if (node && node->inDocument()) {//4.2 Merge
                const WebCore::IntRect& newBounds = node->getRect();
                if ((osw && osh && bounds.width() && bounds.height())
                    && (bounds != newBounds)) {
                    WebCore::FrameView* view = m_mainFrame->view();
                    // force left align if width is not changed while height changed.
                    // the anchorPoint is probably at some white space in the node
                    // which is affected by text wrap around the screen width.
                    const bool leftAlign = (otw != textWrapWidth)
                        && (bounds.width() == newBounds.width())
                        && (bounds.height() != newBounds.height());
                    const float xPercentInDoc =
                        leftAlign ? 0.0 : (float) (anchorX - bounds.x()) / bounds.width();
                    const float xPercentInView =
                        leftAlign ? 0.0 : (float) (anchorX - m_scrollOffsetX) / osw;
                    const float yPercentInDoc = (float) (anchorY - bounds.y()) / bounds.height();
                    const float yPercentInView = (float) (anchorY - m_scrollOffsetY) / osh;
                    showRect(newBounds.x(), newBounds.y(), newBounds.width(),
                             newBounds.height(), view->contentsWidth(),
                             view->contentsHeight(),
                             xPercentInDoc, xPercentInView,
                             yPercentInDoc, yPercentInView);
                }
            }
        }
    } else {
        window->setSize(width, height);
        window->setVisibleSize(screenWidth, screenHeight);
        m_mainFrame->view()->resize(width, height);
        if (width != screenWidth) {
            m_mainFrame->view()->setUseFixedLayout(true);
            m_mainFrame->view()->setFixedLayoutSize(IntSize(width, height));
        } else
            m_mainFrame->view()->setUseFixedLayout(false);
    }

    // update the currently visible screen as perceived by the plugin
    sendPluginVisibleScreen();
}

void WebViewCore::dumpDomTree(bool useFile)
{
#ifdef ANDROID_DOM_LOGGING
    if (useFile)
        gDomTreeFile = fopen(DOM_TREE_LOG_FILE, "w");
    m_mainFrame->document()->showTreeForThis();
    if (gDomTreeFile) {
        fclose(gDomTreeFile);
        gDomTreeFile = 0;
    }
#endif
}

void WebViewCore::dumpRenderTree(bool useFile)
{
#ifdef ANDROID_DOM_LOGGING
    WTF::CString renderDump = WebCore::externalRepresentation(m_mainFrame).utf8();//4.2 Merge
    const char* data = renderDump.data();
    if (useFile) {
        gRenderTreeFile = fopen(RENDER_TREE_LOG_FILE, "w");
        DUMP_RENDER_LOGD("%s", data);
        fclose(gRenderTreeFile);
        gRenderTreeFile = 0;
    } else {
        // adb log can only output 1024 characters, so write out line by line.
        // exclude '\n' as adb log adds it for each output.
        int length = renderDump.length();
        for (int i = 0, last = 0; i < length; i++) {
            if (data[i] == '\n') {
                if (i != last)
                    DUMP_RENDER_LOGD("%.*s", (i - last), &(data[last]));
                last = i + 1;
            }
        }
    }
#endif
}

HTMLElement* WebViewCore::retrieveElement(int x, int y,
    const QualifiedName& tagName)
{
    HitTestResult hitTestResult = m_mainFrame->eventHandler()
        ->hitTestResultAtPoint(IntPoint(x, y), false, false,
        DontHitTestScrollbars, HitTestRequest::Active | HitTestRequest::ReadOnly,
        IntSize(1, 1));
    if (!hitTestResult.innerNode() || !hitTestResult.innerNode()->inDocument()) {
        ALOGE("Should not happen: no in document Node found");
        return 0;
    }
    const ListHashSet<RefPtr<Node> >& list = hitTestResult.rectBasedTestResult();
    if (list.isEmpty()) {
        ALOGE("Should not happen: no rect-based-test nodes found");
        return 0;
    }
    Node* node = hitTestResult.innerNode();
    Node* element = node;
    while (element && (!element->isElementNode()
        || !element->hasTagName(tagName))) {
        element = element->parentNode();
    }
    return static_cast<WebCore::HTMLElement*>(element);
}

HTMLAnchorElement* WebViewCore::retrieveAnchorElement(int x, int y)
{
    return static_cast<HTMLAnchorElement*>
        (retrieveElement(x, y, HTMLNames::aTag));
}

HTMLImageElement* WebViewCore::retrieveImageElement(int x, int y)
{
    return static_cast<HTMLImageElement*>
        (retrieveElement(x, y, HTMLNames::imgTag));
}

WTF::String WebViewCore::retrieveHref(int x, int y)
{
    // TODO: This is expensive, cache
    HitTestResult result = m_mainFrame->eventHandler()->hitTestResultAtPoint(IntPoint(x, y),
                false, false, DontHitTestScrollbars, HitTestRequest::Active | HitTestRequest::ReadOnly, IntSize(1, 1));
    return result.absoluteLinkURL();
}

WTF::String WebViewCore::retrieveAnchorText(int x, int y)
{
    WebCore::HTMLAnchorElement* anchor = retrieveAnchorElement(x, y);
    return anchor ? anchor->text() : WTF::String();
}

WTF::String WebViewCore::retrieveImageSource(int x, int y)
{
    // TODO: This is expensive, cache
    HitTestResult result = m_mainFrame->eventHandler()->hitTestResultAtPoint(IntPoint(x, y),
                false, false, DontHitTestScrollbars, HitTestRequest::Active | HitTestRequest::ReadOnly, IntSize(1, 1));
    return result.absoluteImageURL();
}

WTF::String WebViewCore::requestLabel(WebCore::Frame* frame,
        WebCore::Node* node)
{
    if (node && validNode(m_mainFrame, frame, node)) {
        RefPtr<WebCore::NodeList> list = node->document()->getElementsByTagName("label");
        unsigned length = list->length();
        for (unsigned i = 0; i < length; i++) {
            WebCore::HTMLLabelElement* label = static_cast<WebCore::HTMLLabelElement*>(
                    list->item(i));
            if (label->control() == node) {
                Node* node = label;
                String result;
                while ((node = node->traverseNextNode(label))) {
                    if (node->isTextNode()) {
                        Text* textNode = static_cast<Text*>(node);
                        result += textNode->dataImpl();
                    }
                }
                return result;
            }
        }
    }
    return WTF::String();
}

static bool isContentEditable(const WebCore::Node* node)
{
    if (!node)
        return false;
    return node->isContentEditable();
}

// Returns true if the node is a textfield, textarea, or contentEditable
static bool isTextInput(const WebCore::Node* node)
{
    if (!node)
        return false;
    if (isContentEditable(node))
        return true;
    WebCore::RenderObject* renderer = node->renderer();
    return renderer && (renderer->isTextField() || renderer->isTextArea());
}

//SAMSUNG CHANGE Form Navigation >>

//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES <<
static bool isDateTime(WebCore::Node *node)
{
    if(node && node->hasTagName(HTMLNames::inputTag))
    {
        WebCore::Element* ele = static_cast<Element*>(node);
	const AtomicString &typestr = ele->getAttribute(HTMLNames::typeAttr);		    
//	const WTF::String& typestring = typestr.string();  commented to prevent Tool fix issue.
	if((typestr == "date") || (typestr == "datetime") || (typestr == "datetime-local") || (typestr == "time"))
            return true;
    }
    return false;
}
//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES >>

static bool isFormNavTextInput(WebCore::Node* node)
{
    if (!node)
        return false;
    if (node->hasTagName(HTMLNames::inputTag)) {
        HTMLInputElement *inputElement = static_cast<HTMLInputElement*>(node);
        if (inputElement->readOnly())
            return false;
    }
    if (isContentEditable(node))
        return true;

//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES <<
    if(isDateTime(node))	
        return false;
//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES >>

    WebCore::RenderObject* renderer = node->renderer();
    return renderer && (renderer->isTextField() || renderer->isTextArea());
}

// Returns true if the node is a Select element
static bool isSelectInput(const WebCore::Node* node)
{
    if (!node)
        return false;
    WebCore::RenderObject* renderer = node->renderer();
    if (renderer && (renderer->isMenuList() || renderer->isListBox())) {
        return true;
    }
    else
        return false;
}
//SAMSUNG CHANGE Form Navigation <<

void WebViewCore::revealSelection()
{
    WebCore::Node* focus = currentFocus();
    if (!focus)
        return;
    if (!isTextInput(focus))
        return;
    WebCore::Frame* focusedFrame = focus->document()->frame();
    if (!focusedFrame->page()->focusController()->isActive())
        return;
    focusedFrame->selection()->revealSelection(ScrollAlignment::alignToEdgeIfNeeded);
}

struct TouchNodeData {
    Node* mUrlNode;
    Node* mInnerNode;
    IntRect mBounds;
};

// get the bounding box of the Node
static IntRect getAbsoluteBoundingBox(Node* node) {
 //MPSG100005781 start ++
    if(!node)
        return IntRect(0,0,0,0) ;
 //MPSG100005781 end --
    IntRect rect;
    RenderObject* render = node->renderer();
    if (!render)
        return rect;
    if (render->isRenderInline())
        rect = toRenderInline(render)->linesVisualOverflowBoundingBox();
    else if (render->isBox())
        rect = toRenderBox(render)->visualOverflowRect();
    else if (render->isText())
        rect = toRenderText(render)->linesBoundingBox();
    else
        ALOGE("getAbsoluteBoundingBox failed for node %p, name %s", node, render->renderName());
    FloatPoint absPos = render->localToAbsolute(FloatPoint(), false, true);
    rect.move(absPos.x(), absPos.y());
    return rect;
}

WebCore::Frame* WebViewCore::focusedFrame() const
{
    return m_mainFrame->page()->focusController()->focusedOrMainFrame();
}

VisiblePosition WebViewCore::visiblePositionForContentPoint(int x, int y)
{
    return visiblePositionForContentPoint(IntPoint(x, y));
}

VisiblePosition WebViewCore::visiblePositionForContentPoint(const IntPoint& point)
{
    // Hit test of this kind required for this to work inside input fields
    HitTestRequest request(HitTestRequest::Active
                           | HitTestRequest::MouseMove
                           | HitTestRequest::ReadOnly
                           | HitTestRequest::IgnoreClipping);
    HitTestResult result(point);
    focusedFrame()->document()->renderView()->layer()->hitTest(request, result);

    // Matching the logic in MouseEventWithHitTestResults::targetNode()
    Node* node = result.innerNode();
    if (!node)
        return VisiblePosition();
    Element* element = node->parentElement();
    if (!node->inDocument() && element && element->inDocument())
        node = element;

    return node->renderer()->positionForPoint(result.localPoint());
}

bool WebViewCore::selectWordAt(int x, int y)
{
    HitTestResult hoverResult;
    moveMouse(x, y, &hoverResult);
    if (hoverResult.innerNode()) {
        Node* node = hoverResult.innerNode();
        Frame* frame = node->document()->frame();
        Page* page = m_mainFrame->document()->page();
        page->focusController()->setFocusedFrame(frame);
    }

    IntPoint point = convertGlobalContentToFrameContent(IntPoint(x, y));

    // Hit test of this kind required for this to work inside input fields
    HitTestRequest request(HitTestRequest::Active);
    HitTestResult result(point);

    focusedFrame()->document()->renderView()->layer()->hitTest(request, result);

    // Matching the logic in MouseEventWithHitTestResults::targetNode()
    Node* node = result.innerNode();
    if (!node)
        return false;
    Element* element = node->parentElement();
    if (!node->inDocument() && element && element->inDocument())
        node = element;

    SelectionController* sc = focusedFrame()->selection();
    // SAMSUNG: Text Selection >> 
    sc->setSelection(VisibleSelection());
    // SAMSUNG: Text Selection <<
    bool wordSelected = false;
    if (!sc->contains(point) && (node->isContentEditable() || node->isTextNode()) && !result.isLiveLink()
            && node->dispatchEvent(Event::create(eventNames().selectstartEvent, true, true))) {
        VisiblePosition pos(node->renderer()->positionForPoint(result.localPoint()));
        wordSelected = selectWordAroundPosition(node->document()->frame(), pos);
    }
    return wordSelected;
}

bool WebViewCore::selectWordAroundPosition(Frame* frame, VisiblePosition pos)
{
    VisibleSelection selection(pos);
    selection.expandUsingGranularity(WordGranularity);
    SelectionController* selectionController = frame->selection();
    selection = VisibleSelection(selection.start(), selection.end());//4.2 Merge

    bool wordSelected = false;
    if (selectionController->shouldChangeSelection(selection)) {
        bool allWhitespaces = true;
        RefPtr<Range> firstRange = selection.firstRange();
        String text = firstRange.get() ? firstRange->text() : "";
        for (size_t i = 0; i < text.length(); ++i) {
            if (!isSpaceOrNewline(text[i])) {
                allWhitespaces = false;
                break;
            }
        }
        if (allWhitespaces) {
            VisibleSelection emptySelection(pos);
            selectionController->setSelection(emptySelection);
        } else {
            selectionController->setSelection(selection);
            wordSelected = true;
        }
    }
    return wordSelected;
}

int WebViewCore::platformLayerIdFromNode(Node* node, LayerAndroid** outLayer)
{
    if (!node || !node->renderer())
        return -1;
    RenderLayer* renderLayer = node->renderer()->enclosingLayer();
    while (renderLayer && !renderLayer->isComposited())
        renderLayer = renderLayer->parent();
    if (!renderLayer || !renderLayer->isComposited())
        return -1;
    GraphicsLayer* graphicsLayer = renderLayer->backing()->graphicsLayer();
    if (!graphicsLayer)
        return -1;
    GraphicsLayerAndroid* agl = static_cast<GraphicsLayerAndroid*>(graphicsLayer);
    LayerAndroid* layer = agl->foregroundLayer();
    if (!layer)
        layer = agl->contentLayer();
    if (!layer)
        return -1;
    if (outLayer)
        *outLayer = layer;
    return layer->uniqueId();
}

void WebViewCore::layerToAbsoluteOffset(const LayerAndroid* layer, IntPoint& offset)
{
    while (layer) {
        const SkPoint& pos = layer->getPosition();
        offset.move(pos.fX, pos.fY);
        const IntPoint& scroll = layer->getScrollOffset();
        offset.move(-scroll.x(), -scroll.y());
        layer = static_cast<LayerAndroid*>(layer->getParent());
    }
}

void WebViewCore::setSelectionCaretInfo(SelectText* selectTextContainer,
        const WebCore::Position& pos, const IntPoint& frameOffset,
        SelectText::HandleId handleId, SelectText::HandleType handleType,//4.2 Merge
        int caretRectOffset, EAffinity affinity)
{
    Node* node = pos.anchorNode();
    LayerAndroid* layer = 0;
    int layerId = platformLayerIdFromNode(node, &layer);
    selectTextContainer->setCaretLayerId(handleId, layerId);
    IntPoint offset = frameOffset;
    layerToAbsoluteOffset(layer, offset);
    RenderObject* r = node->renderer();
    RenderText* renderText = toRenderText(r);
    int caretOffset;
    InlineBox* inlineBox;
    pos.getInlineBoxAndOffset(affinity, inlineBox, caretOffset);
    IntRect caretRect = renderText->localCaretRect(inlineBox, caretOffset);
    FloatPoint absoluteOffset = renderText->localToAbsolute(caretRect.location());
    caretRect.setX(absoluteOffset.x() - offset.x() + caretRectOffset);
    caretRect.setY(absoluteOffset.y() - offset.y());
    selectTextContainer->setCaretRect(handleId, caretRect);
    selectTextContainer->setHandleType(handleId, handleType);//4.2 Merge
    selectTextContainer->setTextRect(handleId,
            positionToTextRect(pos, affinity, offset, caretRect));//4.2 Merge
}

bool WebViewCore::isLtr(const Position& position)
{
    InlineBox* inlineBox = 0;
    int caretOffset = 0;
    position.getInlineBoxAndOffset(DOWNSTREAM, inlineBox, caretOffset);
    bool isLtr;
    if (inlineBox)
        isLtr = inlineBox->isLeftToRightDirection();
    else
        isLtr = position.primaryDirection() == LTR;
    return isLtr;
}

// 4.2 Merge BEGIN <<
static Node* findInputParent(Node* node)
{
    Node* testNode = node;
    while (testNode) {
        RenderObject* renderer = testNode->renderer();
        if (renderer
                && (renderer->isTextArea() || renderer->isTextControl())) {
            return testNode;
        }
        testNode = testNode->parentOrHostNode();
    }
    return node;
}
// 4.2 Merge END >>
SelectText* WebViewCore::createSelectText(const VisibleSelection& selection)
{
    bool isCaret = selection.isCaret();
// 4.2 Merge BEGIN <<
    Position base = selection.base();
    Position extent = selection.extent();
// 4.2 Merge END >>
    if (selection.isNone() || (!selection.isContentEditable() && isCaret)
// 4.2 Merge BEGIN <<
            || !base.anchorNode() || !base.anchorNode()->renderer()
            || !extent.anchorNode() || !extent.anchorNode()->renderer())
// 4.2 Merge END >>
        return 0;

    RefPtr<Range> range = selection.firstRange();
    Node* startContainer = range->startContainer();
    Node* endContainer = range->endContainer();

    if (!startContainer || !endContainer)
        return 0;
    if (!isCaret && startContainer == endContainer
            && range->startOffset() == range->endOffset())
        return 0;

    IntPoint frameOffset = convertGlobalContentToFrameContent(IntPoint());
    SelectText* selectTextContainer = new SelectText();
    if (isCaret) {
// 4.2 Merge BEGIN <<
        setSelectionCaretInfo(selectTextContainer, base, frameOffset,
                SelectText::BaseHandle, SelectText::CenterHandle, 0,
                selection.affinity());
        setSelectionCaretInfo(selectTextContainer, base, frameOffset,
                SelectText::ExtentHandle, SelectText::CenterHandle, 0,
                selection.affinity());
// 4.2 Merge END >>
    } else {
// 4.2 Merge BEGIN <<
        bool isBaseLtr = isLtr(base);
        bool isBaseStart = comparePositions(base, extent) <= 0;
        int baseOffset = isBaseLtr ? 0 : -1;
        SelectText::HandleType baseHandleType = (isBaseLtr == isBaseStart)
                ? SelectText::LeftHandle : SelectText::RightHandle;
        EAffinity affinity = selection.affinity();
        setSelectionCaretInfo(selectTextContainer, base, frameOffset,
                SelectText::BaseHandle, baseHandleType, baseOffset, affinity);
        bool isExtentLtr = isLtr(extent);
        int extentOffset = isExtentLtr ? 0 : -1;
        SelectText::HandleType extentHandleType = (isExtentLtr == isBaseStart)
                ? SelectText::RightHandle : SelectText::LeftHandle;
        setSelectionCaretInfo(selectTextContainer, extent, frameOffset,
                SelectText::ExtentHandle, extentHandleType, extentOffset, affinity);
        IntRect clipRect;
        if (selection.isContentEditable()) {
            Node* editable = findInputParent(base.anchorNode());
            RenderObject* render = editable->renderer();
            if (render && render->isBox() && !render->isBody()) {
                RenderBox* renderBox = toRenderBox(render);
                clipRect = renderBox->clientBoxRect();
                FloatPoint pos = renderBox->localToAbsolute(clipRect.location());
                clipRect.setX(pos.x());
                clipRect.setY(pos.y());
            }
        }
// 4.2 Merge END >>

        Node* stopNode = range->pastLastNode();
        for (Node* node = range->firstNode(); node != stopNode; node = node->traverseNextNode()) {
            RenderObject* r = node->renderer();
            if (!r || !r->isText() || r->style()->visibility() != VISIBLE)
                continue;
//SAMSUNG - Google Text Selection >>
		Node* shadowAncestor = r->node()->shadowAncestorNode();

		IntRect TextFieldBox=IntRect(0,0,0,0);

		if (shadowAncestor &&  shadowAncestor->renderer()->isTextArea())
		{
			TextFieldBox = shadowAncestor->renderer()->absoluteBoundingBoxRect();
		}

 //SAMSUNG - Google Text Selection <<
            RenderText* renderText = toRenderText(r);
            int startOffset = node == startContainer ? range->startOffset() : 0;
            int endOffset = node == endContainer ? range->endOffset() : numeric_limits<int>::max();
            LayerAndroid* layer = 0;
            int layerId = platformLayerIdFromNode(node, &layer);
            Vector<IntRect> rects;
            renderText->absoluteRectsForRange(rects, startOffset, endOffset, true);
//SAMSUNG - Google Text Selection >>
	   if (!TextFieldBox.isEmpty())
	   {
	        for (size_t i = 0; i < rects.size(); i++) {
			rects[i].intersect(TextFieldBox);
	        }
//4.2 Merge BEGIN <<
//Following function removed ans replaced
	            //selectTextContainer->addHighlightRegion(layer, rects, frameOffset);
            selectTextContainer->addHighlightRegion(layer, rects, frameOffset,
                    clipRect);
//4.2 Merge END <<
	   }
	   else
//SAMSUNG - Google Text Selection <<
	   {
	            selectTextContainer->addHighlightRegion(layer, rects, frameOffset);
	   }
        }
    }
    selectTextContainer->setText(range->text());
    return selectTextContainer;
}

IntRect WebViewCore::positionToTextRect(const Position& position,
        EAffinity affinity, const WebCore::IntPoint& offset, const IntRect& caretRect)//4.2 Merge
{
    IntRect textRect = caretRect;//4.2 Merge
    InlineBox* inlineBox;
    int offsetIndex;
    position.getInlineBoxAndOffset(affinity, inlineBox, offsetIndex);
    if (inlineBox && inlineBox->isInlineTextBox()) {
        InlineTextBox* box = static_cast<InlineTextBox*>(inlineBox);
        RootInlineBox* root = box->root();
        RenderText* renderText = box->textRenderer();
        int left = root->logicalLeft();
        int width = root->logicalWidth();
        int top = root->selectionTop();
        int height = root->selectionHeight();

        if (!renderText->style()->isHorizontalWritingMode()) {
            swap(left, top);
            swap(width, height);
        }
        FloatPoint origin(left, top);
        FloatPoint absoluteOrigin = renderText->localToAbsolute(origin);

        textRect.setX(absoluteOrigin.x() - offset.x());
        textRect.setWidth(width);
        textRect.setY(absoluteOrigin.y() - offset.y());
        textRect.setHeight(height);
    }
    return textRect;
}

IntPoint WebViewCore::convertGlobalContentToFrameContent(const IntPoint& point, WebCore::Frame* frame)
{
    if (!frame) frame = focusedFrame();
    IntPoint frameOffset(-m_scrollOffsetX, -m_scrollOffsetY);
    frameOffset = frame->view()->windowToContents(frameOffset);
    return IntPoint(point.x() + frameOffset.x(), point.y() + frameOffset.y());
}

VisiblePosition WebViewCore::trimSelectionPosition(const VisiblePosition &start,
        const VisiblePosition& stop)//4.2 Merge
{
    int direction = comparePositions(start, stop);
    if (direction == 0)
        return start;
    bool forward = direction < 0;
//    EAffinity affinity = forward ? DOWNSTREAM : UPSTREAM;//4.2 Merge removed in 4.2
    bool move;
    VisiblePosition pos = start;//4.2 Merge
    bool movedTooFar = false;
    do {
        move = true;
        Node* node = pos.deepEquivalent().anchorNode();//4.2 Merge
        if (node && node->isTextNode() && node->renderer()) {
            RenderText *textRenderer = toRenderText(node->renderer());
            move = !textRenderer->textLength();
        }
        if (move) {
            VisiblePosition nextPos = forward ? pos.next() : pos.previous();//4.2 Merge
            movedTooFar = nextPos.isNull() || pos == nextPos
                    || ((comparePositions(nextPos, stop) < 0) != forward);
            pos = nextPos;
        }
    } while (move && !movedTooFar);
    if (movedTooFar)
        pos = stop;
    return pos;
}

void WebViewCore::selectText(SelectText::HandleId handleId, int x, int y)//4.2 Merge
{
    SelectionController* sc = focusedFrame()->selection();
// 4.2 Merge BEGIN <<
    VisibleSelection selection = sc->selection();
    Position base = selection.base();
    Position extent = selection.extent();
    IntPoint dragPoint = convertGlobalContentToFrameContent(IntPoint(x, y));
    VisiblePosition dragPosition(visiblePositionForContentPoint(dragPoint));
// 4.2 Merge END >>

    if (base.isNull() || extent.isNull() || dragPosition.isNull())//4.2 Merge
        return;
// 4.2 Merge BEGIN <<
    bool draggingBase = (handleId == SelectText::BaseHandle);
    if (draggingBase)
        base = dragPosition.deepEquivalent();
    else
        extent = dragPosition.deepEquivalent();
// 4.2 Merge END >>

// 4.2 Merge BEGIN <<
    bool baseIsStart = (comparePositions(base, extent) <= 0);
    Position& start = baseIsStart ? base : extent;
    Position& end = baseIsStart ? extent : base;
    VisiblePosition startPosition(start, selection.affinity());
    VisiblePosition endPosition(end, selection.affinity());
    bool draggingStart = (baseIsStart == draggingBase);
// 4.2 Merge END >>

// 4.2 Merge BEGIN <<
    if (draggingStart) {
        if (selection.isRange()) {
            startPosition = trimSelectionPosition(startPosition, endPosition);
            if ((startPosition != endPosition) && isEndOfBlock(startPosition)) {
                // Ensure startPosition is not at end of block
                VisiblePosition nextStartPosition(startPosition.next());
                if (nextStartPosition.isNotNull())
                    startPosition = nextStartPosition;
            }
        }
        startPosition = endPosition.honorEditableBoundaryAtOrAfter(startPosition);
        if (startPosition.isNull())
            return;
        start = startPosition.deepEquivalent();
        if (selection.isCaret())
            end = start;
    } else {
        if (selection.isRange()) {
            endPosition = trimSelectionPosition(endPosition, startPosition);
            if ((start != end) && isStartOfBlock(endPosition)) {
                // Ensure endPosition is not at start of block
                VisiblePosition prevEndPosition(endPosition.previous());
                if (!prevEndPosition.isNull())
                    endPosition = prevEndPosition;
            }
        }
        endPosition = startPosition.honorEditableBoundaryAtOrAfter(endPosition);
        if (endPosition.isNull())
            return;
        end = endPosition.deepEquivalent();
        if (selection.isCaret())
            start = end;
    }

    selection = VisibleSelection(base, extent);

//    if (sc->isContentEditable()) {
//        startPosition = sc->selection().visibleStart().honorEditableBoundaryAtOrAfter(startPosition);
//        endPosition = sc->selection().visibleEnd().honorEditableBoundaryAtOrBefore(endPosition);
//        if (startPosition.isNull() || endPosition.isNull()) {
//            return;
//        }
//    }

//    // Ensure startPosition is not at end of block
//    if (startPosition != endPosition && isEndOfBlock(startPosition)) {
//        VisiblePosition nextStartPosition(startPosition.next());
//        if (!nextStartPosition.isNull())
//            startPosition = nextStartPosition;
//    }
//    // Ensure endPosition is not at start of block
//    if (startPosition != endPosition && isStartOfBlock(endPosition)) {
//        VisiblePosition prevEndPosition(endPosition.previous());
//        if (!prevEndPosition.isNull())
//            endPosition = prevEndPosition;
//    }

//    Position start = startPosition.deepEquivalent();
//    Position end = endPosition.deepEquivalent();
//    start = trimSelectionPosition(start, end);
//    end = trimSelectionPosition(end, start);
//    VisibleSelection selection(start, end);
// 4.2 Merge END >>
    // Only allow changes between caret positions or to text selection.
    bool selectChangeAllowed = (!selection.isCaret() || sc->isCaret());
    if (selectChangeAllowed && sc->shouldChangeSelection(selection))
        sc->setSelection(selection);
}

bool WebViewCore::nodeIsClickableOrFocusable(Node* node)
{
    if (!node)
        return false;
    if (node->disabled())
        return false;
    if (!node->inDocument())
        return false;
    if (!node->renderer() || node->renderer()->style()->visibility() != VISIBLE)
        return false;
    return node->supportsFocus()
            || node->hasEventListeners(eventNames().clickEvent)
            || node->hasEventListeners(eventNames().mousedownEvent)
            || node->hasEventListeners(eventNames().mouseupEvent)
            || node->hasEventListeners(eventNames().mouseoverEvent);
}

// get the highlight rectangles for the touch point (x, y) with the slop
AndroidHitTestResult WebViewCore::hitTestAtPoint(int x, int y, int slop, bool doMoveMouse)
{
    if (doMoveMouse)
        moveMouse(x, y, 0, true);
    HitTestResult hitTestResult = m_mainFrame->eventHandler()->hitTestResultAtPoint(IntPoint(x, y),
            false, false, DontHitTestScrollbars, HitTestRequest::Active | HitTestRequest::ReadOnly, IntSize(slop, slop));
    AndroidHitTestResult androidHitResult(this, hitTestResult);
    if (!hitTestResult.innerNode() || !hitTestResult.innerNode()->inDocument()) {
        ALOGE("Should not happen: no in document Node found");
        return androidHitResult;
    }
    const ListHashSet<RefPtr<Node> >& list = hitTestResult.rectBasedTestResult();
    if (list.isEmpty()) {
        ALOGE("Should not happen: no rect-based-test nodes found");
        return androidHitResult;
    }
    androidHitResult.setDoubleTapNode(hitTestResult.innerNode());            //SAMSUNG_CHANGES - MPSG100006065 
    Frame* frame = hitTestResult.innerNode()->document()->frame();
    Vector<TouchNodeData> nodeDataList;
    if (hitTestResult.innerNode() != hitTestResult.innerNonSharedNode()
            && hitTestResult.innerNode()->hasTagName(WebCore::HTMLNames::areaTag)) {
        HTMLAreaElement* area = static_cast<HTMLAreaElement*>(hitTestResult.innerNode());
        androidHitResult.hitTestResult().setURLElement(area);
        androidHitResult.highlightRects().append(area->computeRect(
                hitTestResult.innerNonSharedNode()->renderer()));
        return androidHitResult;
    }
    ListHashSet<RefPtr<Node> >::const_iterator last = list.end();
    for (ListHashSet<RefPtr<Node> >::const_iterator it = list.begin(); it != last; ++it) {
        // TODO: it seems reasonable to not search across the frame. Isn't it?
        // if the node is not in the same frame as the innerNode, skip it
        if (it->get()->document()->frame() != frame)
            continue;
        // traverse up the tree to find the first node that needs highlight
        bool found = false;
        Node* eventNode = it->get();
        Node* innerNode = eventNode;
        while (eventNode) {
            RenderObject* render = eventNode->renderer();
            if (render && (render->isBody() || render->isRenderView()))
                break;
            if (nodeIsClickableOrFocusable(eventNode)) {
                found = true;
                break;
            }
            // the nodes in the rectBasedTestResult() are ordered based on z-index during hit testing.
            // so do not search for the eventNode across explicit z-index border.
            // TODO: this is a hard one to call. z-index is quite complicated as its value only
            // matters when you compare two RenderLayer in the same hierarchy level. e.g. in
            // the following example, "b" is on the top as its z level is the highest. even "c"
            // has 100 as z-index, it is still below "d" as its parent has the same z-index as
            // "d" and logically before "d". Of course "a" is the lowest in the z level.
            //
            // z-index:auto "a"
            //   z-index:2 "b"
            //   z-index:1
            //     z-index:100 "c"
            //   z-index:1 "d"
            //
            // If the fat point touches everyone, the order in the list should be "b", "d", "c"
            // and "a". When we search for the event node for "b", we really don't want "a" as
            // in the z-order it is behind everything else.
            if (render && !render->style()->hasAutoZIndex())
                break;
            eventNode = eventNode->parentNode();
        }
        // didn't find any eventNode, skip it
        if (!found)
            continue;
        // first quick check whether it is a duplicated node before computing bounding box
        Vector<TouchNodeData>::const_iterator nlast = nodeDataList.end();
        for (Vector<TouchNodeData>::const_iterator n = nodeDataList.begin(); n != nlast; ++n) {
            // found the same node, skip it
            if (eventNode == n->mUrlNode) {
                found = false;
                break;
            }
        }
        if (!found)
            continue;
        // next check whether the node is fully covered by or fully covering another node.
        found = false;
        IntRect rect = getAbsoluteBoundingBox(eventNode);
        if (rect.isEmpty()) {
            // if the node's bounds is empty and it is not a ContainerNode, skip it.
            if (!eventNode->isContainerNode())
                continue;
            // if the node's children are all positioned objects, its bounds can be empty.
            // Walk through the children to find the bounding box.
            Node* child = static_cast<const ContainerNode*>(eventNode)->firstChild();
            while (child) {
                IntRect childrect;
                if (child->renderer())
                    childrect = getAbsoluteBoundingBox(child);
                if (!childrect.isEmpty()) {
                    rect.unite(childrect);
                    child = child->traverseNextSibling(eventNode);
                } else
                    child = child->traverseNextNode(eventNode);
            }
        }
        for (int i = nodeDataList.size() - 1; i >= 0; i--) {
            TouchNodeData n = nodeDataList.at(i);
            // the new node is enclosing an existing node, skip it
            if (rect.contains(n.mBounds)) {
                found = true;
                break;
            }
            // the new node is fully inside an existing node, remove the existing node
            if (n.mBounds.contains(rect))
                nodeDataList.remove(i);
        }
        if (!found) {
            TouchNodeData newNode;
            newNode.mUrlNode = eventNode;
            newNode.mBounds = rect;
            newNode.mInnerNode = innerNode;
            nodeDataList.append(newNode);
        }
    }
    if (!nodeDataList.size()) {
        androidHitResult.searchContentDetectors();
        return androidHitResult;
    }
    // finally select the node with the largest overlap with the fat point
    TouchNodeData final;
    final.mUrlNode = 0;
    IntPoint docPos = frame->view()->windowToContents(m_mousePos);
    IntRect testRect(docPos.x() - slop, docPos.y() - slop, 2 * slop + 1, 2 * slop + 1);
    int area = 0;
    Vector<TouchNodeData>::const_iterator nlast = nodeDataList.end();
    for (Vector<TouchNodeData>::const_iterator n = nodeDataList.begin(); n != nlast; ++n) {
        IntRect rect = n->mBounds;
        rect.intersect(testRect);
        int a = rect.width() * rect.height();
        if (a > area || !final.mUrlNode) {
            final = *n;
            area = a;
        }
    }
    // SAMSUNG CHANGE MPSG100006305 >>
    if (final.mUrlNode && final.mUrlNode->isContainerNode() && !final.mUrlNode->isLink()) {
        WebCore::Node *child = static_cast<const ContainerNode*>(final.mUrlNode)->firstChild();

        while(child) {
              ALOGV("Child  %s ",child->nodeName().latin1().data());
              if( nodeIsClickableOrFocusable(child) ) {
                 ALOGV("Dont draw focus ring since children are focusable or have trigger ie %s ",child->nodeName().latin1().data());
                 androidHitResult.dontDrawTapHighlight();
                 break;
              }
              child = child->traverseNextNode(final.mUrlNode);
        }
    }
    // SAMSUNG CHANGE MPSG100006305 <<
    // now get the node's highlight rectangles in the page coordinate system
    if (final.mUrlNode) {
        // Update innerNode and innerNonSharedNode
        androidHitResult.hitTestResult().setInnerNode(final.mInnerNode);
        androidHitResult.hitTestResult().setInnerNonSharedNode(final.mInnerNode);
        if (final.mUrlNode->isElementNode()) {
            // We found a URL element. Update the hitTestResult
            androidHitResult.setURLElement(static_cast<Element*>(final.mUrlNode));
        } else {
            androidHitResult.setURLElement(0);
        }
        Vector<IntRect>& highlightRects = androidHitResult.highlightRects();
        if (doMoveMouse && highlightRects.size() > 0) {
            // adjust m_mousePos if it is not inside the returned highlight
            // rectangles
            IntRect foundIntersection;
            IntRect inputRect = IntRect(x - slop, y - slop,
                                        slop * 2 + 1, slop * 2 + 1);
            for (size_t i = 0; i < highlightRects.size(); i++) {
                IntRect& hr = highlightRects[i];
                IntRect test = inputRect;
                test.intersect(hr);
                if (!test.isEmpty()) {
                    foundIntersection = test;
                    break;
                }
            }
            if (!foundIntersection.isEmpty() && !foundIntersection.contains(x, y)) {
                IntPoint pt = foundIntersection.center();
                moveMouse(pt.x(), pt.y(), 0, true);
            }
        }
    } else {
        androidHitResult.searchContentDetectors();
    }
    return androidHitResult;
}

///////////////////////////////////////////////////////////////////////////////

void WebViewCore::addPlugin(PluginWidgetAndroid* w)
{
//    SkDebugf("----------- addPlugin %p", w);
    /* The plugin must be appended to the end of the array. This ensures that if
       the plugin is added while iterating through the array (e.g. sendEvent(...))
       that the iteration process is not corrupted.
     */
    *m_plugins.append() = w;
}

void WebViewCore::removePlugin(PluginWidgetAndroid* w)
{
//    SkDebugf("----------- removePlugin %p", w);
    int index = m_plugins.find(w);
    if (index < 0) {
        SkDebugf("--------------- pluginwindow not found! %p\n", w);
    } else {
        m_plugins.removeShuffle(index);
    }
}

bool WebViewCore::isPlugin(PluginWidgetAndroid* w) const
{
    return m_plugins.find(w) >= 0;
}

void WebViewCore::invalPlugin(PluginWidgetAndroid* w)
{
    const double PLUGIN_INVAL_DELAY = 1.0 / 60;

    if (!m_pluginInvalTimer.isActive()) {
        m_pluginInvalTimer.startOneShot(PLUGIN_INVAL_DELAY);
    }
}

void WebViewCore::drawPlugins()
{
    SkRegion inval; // accumualte what needs to be redrawn
    PluginWidgetAndroid** iter = m_plugins.begin();
    PluginWidgetAndroid** stop = m_plugins.end();

    for (; iter < stop; ++iter) {
        PluginWidgetAndroid* w = *iter;
        SkIRect dirty;
        if (w->isDirty(&dirty)) {
            w->draw();
            inval.op(dirty, SkRegion::kUnion_Op);
        }
    }

    if (!inval.isEmpty()) {
        // inval.getBounds() is our rectangle
        const SkIRect& bounds = inval.getBounds();
        WebCore::IntRect r(bounds.fLeft, bounds.fTop,
                           bounds.width(), bounds.height());
        this->viewInvalidate(r);
    }
}

void WebViewCore::notifyPluginsOnFrameLoad(const Frame* frame) {
    // if frame is the parent then notify all plugins
    if (!frame->tree()->parent()) {
        // trigger an event notifying the plugins that the page has loaded
        ANPEvent event;
        SkANP::InitEvent(&event, kLifecycle_ANPEventType);
        event.data.lifecycle.action = kOnLoad_ANPLifecycleAction;
        sendPluginEvent(event);
        // trigger the on/off screen notification if the page was reloaded
        sendPluginVisibleScreen();
    }
    // else if frame's parent has completed
    else if (!frame->tree()->parent()->loader()->isLoading()) {
        // send to all plugins who have this frame in their heirarchy
        PluginWidgetAndroid** iter = m_plugins.begin();
        PluginWidgetAndroid** stop = m_plugins.end();
        for (; iter < stop; ++iter) {
            Frame* currentFrame = (*iter)->pluginView()->parentFrame();
            while (currentFrame) {
                if (frame == currentFrame) {
                    ANPEvent event;
                    SkANP::InitEvent(&event, kLifecycle_ANPEventType);
                    event.data.lifecycle.action = kOnLoad_ANPLifecycleAction;
                    (*iter)->sendEvent(event);

                    // trigger the on/off screen notification if the page was reloaded
                    ANPRectI visibleRect;
                    getVisibleScreen(visibleRect);
                    (*iter)->setVisibleScreen(visibleRect, m_scale);

                    break;
                }
                currentFrame = currentFrame->tree()->parent();
            }
        }
    }
}

void WebViewCore::getVisibleScreen(ANPRectI& visibleRect)
{
    visibleRect.left = m_scrollOffsetX;
    visibleRect.top = m_scrollOffsetY;
    visibleRect.right = m_scrollOffsetX + m_screenWidth;
    visibleRect.bottom = m_scrollOffsetY + m_screenHeight;
}

void WebViewCore::sendPluginVisibleScreen()
{
    /* We may want to cache the previous values and only send the notification
       to the plugin in the event that one of the values has changed.
     */

    ANPRectI visibleRect;
    getVisibleScreen(visibleRect);

    PluginWidgetAndroid** iter = m_plugins.begin();
    PluginWidgetAndroid** stop = m_plugins.end();
    for (; iter < stop; ++iter) {
        (*iter)->setVisibleScreen(visibleRect, m_scale);
    }
}

void WebViewCore::sendPluginSurfaceReady()
{
    PluginWidgetAndroid** iter = m_plugins.begin();
    PluginWidgetAndroid** stop = m_plugins.end();
    for (; iter < stop; ++iter) {
        (*iter)->checkSurfaceReady();
    }
}

void WebViewCore::sendPluginEvent(const ANPEvent& evt)
{
    /* The list of plugins may be manipulated as we iterate through the list.
       This implementation allows for the addition of new plugins during an
       iteration, but may fail if a plugin is removed. Currently, there are not
       any use cases where a plugin is deleted while processing this loop, but
       if it does occur we will have to use an alternate data structure and/or
       iteration mechanism.
     */
    for (int x = 0; x < m_plugins.count(); x++) {
        m_plugins[x]->sendEvent(evt);
    }
}

PluginWidgetAndroid* WebViewCore::getPluginWidget(NPP npp)
{
    PluginWidgetAndroid** iter = m_plugins.begin();
    PluginWidgetAndroid** stop = m_plugins.end();
    for (; iter < stop; ++iter) {
        if ((*iter)->pluginView()->instance() == npp) {
            return (*iter);
        }
    }
    return 0;
}

static PluginView* nodeIsPlugin(Node* node) {
    RenderObject* renderer = node->renderer();
    if (renderer && renderer->isWidget()) {
        Widget* widget = static_cast<RenderWidget*>(renderer)->widget();
        if (widget && widget->isPluginView())
            return static_cast<PluginView*>(widget);
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

// Update mouse position
void WebViewCore::moveMouse(int x, int y, HitTestResult* hoveredNode, bool isClickCandidate)
{
    // mouse event expects the position in the window coordinate
    m_mousePos = WebCore::IntPoint(x - m_scrollOffsetX, y - m_scrollOffsetY);
    if (isClickCandidate)
        m_mouseClickPos = m_mousePos;
    // validNode will still return true if the node is null, as long as we have
    // a valid frame.  Do not want to make a call on frame unless it is valid.
    WebCore::PlatformMouseEvent mouseEvent(m_mousePos, m_mousePos,
        WebCore::NoButton, WebCore::MouseEventMoved, 1, false, false, false,
        false, WTF::currentTime());
    m_mainFrame->eventHandler()->handleMouseMoveEvent(mouseEvent, hoveredNode);
}


//SAMSUNG ADVANCED TEXT SELECTION - BEGIN
WTF::String WebViewCore::getSelectedText()
{
 // WebCore::Frame* frame = m_mainFrame;
 	WebCore::Frame* frame = focusedFrame();
    WTF::String str = frame->editor()->selectedText();
    return str;
}
//SAMSUNG ADVANCED TEXT SELECTION - END
// SAMSUNG CHANGE ++ GET_INPUT_TEXT_BOUNDS
WebCore::IntRect WebViewCore::getInputTextBounds()
{
	WebCore::Node* focus = currentFocus();
	if (!focus) {
		clearTextEntry();
		return WebCore::IntRect();
	}
	WebCore::RenderTextControl* renderText = toRenderTextControl(focus);
	if (!renderText) {
		clearTextEntry();
		return WebCore::IntRect();
	}

	LayerAndroid* layer = 0;
	platformLayerIdFromNode(focus, &layer);
	return absoluteClientRect(focus, layer);//4.2 Merge
}

// SAMSUNG CHANGE --
//SISO_HTMLCOMPOSER start
void WebViewCore::insertContent(WTF::String content,int newcursorpostion, bool composing, Vector<CompositionUnderline> undVec,
                                int& startOffset,int& endOffset)
{
    ALOGV("insertContent enter %d",composing);

    SelectionController* selectionContrler = m_mainFrame->selection();
    if(selectionContrler != NULL && !(selectionContrler->isNone()))
    {
        Position m_endSelPos;
        if(composing == true)
        {
            m_mainFrame->editor()->setUnderLineUpdateFlag(true);
            ALOGV(" undVec %d len " ,  undVec.size());
            undelineVec = undVec;
            ALOGV(" undelineVec %d len " ,  undelineVec.size());
            m_mainFrame->editor()->setComposition(content, undelineVec , content.length(), content.length());
            startOffset =0;
            endOffset = content.length();
        }
        else
        {
            m_mainFrame->editor()->confirmComposition(content);
            startOffset = -1;
            endOffset = -1;
            m_mainFrame->editor()->setUnderLineUpdateFlag(false);
        }

        m_endSelPos = selectionContrler->selection().visibleEnd().deepEquivalent();

        if(newcursorpostion > 0)
        {
            int forWrdMove = newcursorpostion - 1;
            if(forWrdMove > 0)
            {
                Position tempPosition = getNextPosition(m_endSelPos , forWrdMove) ;
                selectionContrler->setSelection(VisibleSelection(tempPosition));
            }
        }
/*        else
        {
            int backMove = newcursorpostion < 0 ? -newcursorpostion : 0;
            backMove = content.length() - backMove;
            Position tempPosition = getPreviousPosition(m_endSelPos , backMove) ;
            selectionContrler->setSelection(VisibleSelection(tempPosition));

        }*/

    }
}

Position WebViewCore::getPreviousPosition(Position& pos , int prevNum)
{
    Node* positionNode = pos.anchorNode(); 
    RenderObject* renderer = 0;

    int cnt = 0;

    if(positionNode)
        renderer= positionNode->renderer();
    else
        ALOGV("getPreviousPosition position node is null");

    if (renderer && renderer->isImage() && Position::PositionIsAfterAnchor == pos.anchorType() ) {
        ALOGV("getPreviousPosition the node before cursor is an image so returning");
        return pos;
    }
    else{
        ALOGV("getPreviousPosition the node before cursor is not an image");
    
    }

    if(pos.atStartOfTree())
        return pos;

    if(!pos.anchorNode()->offsetInCharacters() )
    {
        if(pos.anchorNode()->renderer() && pos.anchorNode()->renderer()->isBR() && 0==pos.deprecatedEditingOffset()) {
            __android_log_print(ANDROID_LOG_DEBUG,"webcoreglue", " &&&&&&&&&&&&&&&&&&&&&& node()->renderer()->isBR() pos %s %d " , pos.anchorNode()->nodeName().utf8().data() , pos.deprecatedEditingOffset());
            cnt++;
        }

        ALOGV("!pos.anchorNode()->offsetInCharacters() getPreviousPosition");

        if( cnt < prevNum ) {
            Position prevPos = pos.previous();

            return getPreviousPosition(prevPos , prevNum - cnt);
        } else {
            return pos;
        }
    }
    Position retVal = pos;
    while(cnt < prevNum)
    {
        retVal = retVal.previous();

        positionNode = retVal.anchorNode();

        if( positionNode )
            renderer = positionNode->renderer();
        else
            __android_log_print(ANDROID_LOG_DEBUG,"webcoreglue", "getPreviousPosition position node is null");

        if( renderer && renderer->isImage() ) {
            __android_log_print(ANDROID_LOG_DEBUG,"webcoreglue", "getPreviousPosition the node before cursor is an image so returning");
            return retVal.next();     
        } else {
            ALOGV("getPreviousPosition the node before cursor is not an image");
        }

        if(retVal.anchorNode()->offsetInCharacters() )
        {
            cnt++;
            ALOGV("getPreviousPosition retVal.deprecatedEditingOffset() = [%d]", retVal.deprecatedEditingOffset());
			if(cnt != prevNum && retVal.deprecatedEditingOffset() == 0)
            {

                cnt--;
            }
        } else {
            if(retVal.anchorNode()->renderer() && retVal.anchorNode()->renderer()->isBR() && 1==retVal.deprecatedEditingOffset() ) {
                ALOGV("getPreviousPosition node()->renderer()->isBR() retVal %s %d " , retVal.anchorNode()->nodeName().utf8().data() , retVal.deprecatedEditingOffset());
                cnt++;
            }
        }

        if(retVal.atStartOfTree())
            break;
    }
    return retVal;
}

Position WebViewCore::getNextPosition(Position& pos ,int nextNum)
{
    int cnt = 0;

    Node* positionNode = pos.anchorNode(); 
    RenderObject* renderer = 0;
    if(positionNode)
        renderer= positionNode->renderer();
    else
        ALOGV("getPreviousPosition position node is null");

    if (renderer && renderer->isImage() && Position::PositionIsBeforeAnchor == pos.anchorType() ) {
        ALOGV("getPreviousPosition the node before cursor is an image so returning");
        return pos;
    }
    else{
        ALOGV("getPreviousPosition the node before cursor is not an image");
    }

    if(pos.atEndOfTree())
        return pos;

    if(!pos.anchorNode()->offsetInCharacters()  )
    {
        if( pos.anchorNode()->renderer() && pos.anchorNode()->renderer()->isBR() && 1==pos.deprecatedEditingOffset() ) {
            __android_log_print(ANDROID_LOG_DEBUG,"webcoreglue", "!pos.anchorNode()->offsetInCharacters() BR is checked. So text count ++");
            cnt++;
        }

        ALOGV("!pos.anchorNode()->offsetInCharacters() getNextPosition");

        if( cnt < nextNum ) {
            Position nextPos = pos.next();

            return getNextPosition(nextPos , nextNum-cnt);
        } else {
            return pos;
        }
    }

    Position retVal = pos;

    while(cnt < nextNum)
    {
        retVal = retVal.next();

        positionNode = retVal.anchorNode();

        if(positionNode)
            renderer= positionNode->renderer();
        else {
            __android_log_print(ANDROID_LOG_DEBUG,"webcoreglue", "getNextPosition position node is null");
            continue;
        }

        if (renderer && renderer->isImage()) {
            __android_log_print(ANDROID_LOG_DEBUG,"webcoreglue", "getNextPosition the node before cursor is an image so returning");

            while ( !retVal.anchorNode()->offsetInCharacters() ) {
                retVal = retVal.previous();
            }
            return retVal;
        }
        else{
            ALOGV("getNextPosition the node before cursor is not an image");
        }

        if(positionNode->offsetInCharacters()  )
        {
            if( 0 == retVal.deprecatedEditingOffset() && positionNode->previousSibling() ) {
                ALOGV("getNextPosition if previousSibling == textNode, \"\\n\" is not added.");
            } else {
                cnt++;
            }

        } else {
            if(positionNode->renderer() && positionNode->renderer()->isBR() && 1==retVal.deprecatedEditingOffset() ) {
                ALOGV("getNextPosition node()->renderer()->isBR() retVal %s %d " , positionNode->nodeName().utf8().data() , retVal.deprecatedEditingOffset());
                cnt++;
            }
        }

        if(retVal.atEndOfTree())
            break;
    }
    return retVal;
}

void WebViewCore::simulateDelKeyForCount(int count)
{
    ALOGV("simulateDelKeyForCount enter");

    if( m_mainFrame->editor()->hasComposition() ) {
        ALOGV("simulateDelKeyForCount hasComposition == true");

        m_mainFrame->editor()->deleteBackwardByDecomposingPreviousCharacter(count);

        return;
    }
	
    PlatformKeyboardEvent down(AKEYCODE_DEL, 0, 0, true, false, false, false);
    PlatformKeyboardEvent up(AKEYCODE_DEL, 0, 0, false, false, false, false);
    for(int cnt = 0 ; cnt < count ; cnt++)
    {
        key(down);
        key(up);
    }
    ALOGV("simulateDelKeyForCount exit");
}

void WebViewCore::simulateDelKeyForCount(int count, bool isBackward)
{
    ALOGV("simulateDelKeyForCount enter");

    if( m_mainFrame->editor()->hasComposition() && isBackward) {
        ALOGV("simulateDelKeyForCount hasComposition == true");

        m_mainFrame->editor()->deleteBackwardByDecomposingPreviousCharacter(count);

        return;
    }
	
    Node* anchorNode = NULL;
    if(currentFocus() && currentFocus()->document())
    {
        WebCore::Frame* focusedFrame = currentFocus()->document()->frame(); 
        if(focusedFrame && focusedFrame->selection())
        {
            VisibleSelection selection = focusedFrame->selection()->selection();
            anchorNode = selection.end().anchorNode();
        }
    }

    if( isBackward ) { // BackwardDelete
        PlatformKeyboardEvent down(AKEYCODE_DEL, 0, 0, true, false, false, false);
        PlatformKeyboardEvent up(AKEYCODE_DEL, 0, 0, false, false, false, false);
        for(int cnt = 0 ; cnt < count ; cnt++)
        {
            key(down);
            key(up);
        }
    } else { //ForwardDelete
        PlatformKeyboardEvent down(AKEYCODE_DEL, 0, 0, true, true, false, false);
        PlatformKeyboardEvent up(AKEYCODE_DEL, 0, 0, false, true, false, false);
        for(int cnt = 0 ; cnt < count ; cnt++)
        {
            key(down);
            key(up);
        }
    }

    if(anchorNode && anchorNode->hasTagName(HTMLNames::imgTag))
        contentInvalidateAll();	
    ALOGV("simulateDelKeyForCount exit");
}

WTF::String WebViewCore::getTextAroundCursor(int count , bool isBefore)
{
    ALOGV("getTextAroundCursor enter");
    if(count == -1)
    {
        ALOGV("getTextAroundCursor -1 ");
        SelectionController* selectionContrler = m_mainFrame->selection();
        if(selectionContrler != NULL && !(selectionContrler->isNone()))
        {
            Position pos = selectionContrler->selection().visibleEnd().deepEquivalent();
            IntPoint pt = IntPoint(0, 0);
            Position startPos = m_mainFrame->visiblePositionForPoint(pt).deepEquivalent();

            VisibleSelection newSelection;
            SelectionController newSelectionControler;
            //if (comparePositions(pos , startPos) <= 0)
                    //  newSelection = VisibleSelection(pos, startPos);
                //else
                        newSelection = VisibleSelection(startPos, pos);

            newSelectionControler.setSelection(newSelection);
            PassRefPtr<Range> rangePtr = newSelectionControler.toNormalizedRange();
            WebCore::Range* range = rangePtr.get();
            if(range != NULL)
            {
                WTF::String plainText = range->text();
                plainText.replace(NonBreakingSpaceCharacter, SpaceCharacter);
                ALOGV("getTextAroundCursor -1 ret");
                return plainText;
            }

        }
    }
    else
    {
        SelectionController* frameSelectionContrler = m_mainFrame->selection();
        if(frameSelectionContrler == NULL)
            return "";
        ALOGV("getTextAroundCursor setSelection ent");
        SelectionController newSelection;

        Position m_endSelPos;///////////
        Position m_startSelPos;///////////////////////
        bool isBr = false;
        if(isBefore)
        {
            ALOGV("getTextAroundCursor setSelection Inside isBefore ");
            m_endSelPos = frameSelectionContrler->selection().visibleStart().deepEquivalent();//////////////
            if(m_endSelPos.anchorNode() && m_endSelPos.anchorNode()->renderer() && m_endSelPos.anchorNode()->renderer()->isBR() && m_endSelPos.deprecatedEditingOffset() == 0)
            {
                ALOGV(" &&&&&&&&&&&&&&&&&&&&&& node()->renderer()->isBR()  ");

                //m_endSelPos = Position(m_endSelPos.anchorNode() , Position::PositionIsAfterAnchor);
                isBr = true;
            }
            m_startSelPos = getPreviousPosition(m_endSelPos , count);
        }
        else
        {
            ALOGV("getTextAroundCursor setSelection Inside NOT isBefore ");
            m_startSelPos = frameSelectionContrler->selection().visibleEnd().deepEquivalent();
            if(m_startSelPos.anchorNode() && m_startSelPos.anchorNode()->renderer() && m_startSelPos.anchorNode()->renderer()->isBR() /*&& m_startSelPos.deprecatedEditingOffset() == 0*/)
            {
                ALOGV(" &&&&&&&&&&&&&&&&&&&&&& node()->renderer()->isBR()  ");

                //m_endSelPos = Position(m_endSelPos.anchorNode() , Position::PositionIsAfterAnchor);
                isBr = true;
            }
            m_endSelPos = getNextPosition(m_startSelPos,count);
        }
        RefPtr<Range> rangePtr;
        if(isBr)
        {
            SelectionController newSelection;
            newSelection.setSelection(frameSelectionContrler->selection());
            ALOGV("getTextAroundCursor setSelection exit");
            for(int cnt = 0 ; cnt < count ; cnt++)
            {
                if(isBefore)
                {
                    if(frameSelectionContrler->isRange())
                  newSelection.modify(SelectionController::AlterationMove, DirectionBackward, CharacterGranularity);           
                    newSelection.modify(SelectionController::AlterationExtend, DirectionBackward, CharacterGranularity);
                }           
                else
                {
                    if(frameSelectionContrler->isRange())
                  newSelection.modify(SelectionController::AlterationMove, DirectionForward, CharacterGranularity);
                    newSelection.modify(SelectionController::AlterationExtend, DirectionForward, CharacterGranularity);
                }
            }

            rangePtr = newSelection.toNormalizedRange();
        }
        else
        {
            rangePtr = Range::create(m_mainFrame->document(), m_startSelPos , m_endSelPos);
        }
        ALOGV("getTextAroundCursor getting rangePtr from toNormalizedRange   ");

        ALOGV("getTextAroundCursor getting range from rangePtr  ");
        if(rangePtr == NULL )
        {
            ALOGV("getTextAroundCursor rangePtr  is NULL  ");
            return "";
        }
        WebCore::Range* range = rangePtr.get();
        ALOGV("getTextAroundCursor range->text ent");
        if(range != NULL)
        {
            WTF::String plainText = range->text();
            ALOGV("HTML getTextAroundCursor range->text exit with following plainText %p:  plainText.length()  =  %d  count = %d " ,  plainText.utf8().data() , plainText.length(), count );//+ plainText);
            ALOGV("getTextAroundCursor exit");
            if(plainText.length() > count)
            {
                if(isBefore)
                {
                    plainText = plainText.substring(plainText.length()-count,count);
                }
                else
                {
                    plainText = plainText.substring(0,count);
                }
            }
            ALOGV("HTML getTextAroundCursor rreturns following plainText :  " );//+ plainText);
            return plainText ;
        }
    }
    ALOGV("HTML getTextAroundCursor exit");
    return "";
}

void WebViewCore::updateIMSelection(int curStr , int curEnd){
    m_imStr = curStr;
    m_imEnd = curEnd;
}

int WebViewCore::checkSelectionAtBoundry()
{
        int result = 0;

    SelectionController* frameSelectionContrler = m_mainFrame->selection();
    if(frameSelectionContrler == NULL)
        return result;


    IntPoint pt = IntPoint(0, 0);
    VisiblePosition startPos = m_mainFrame->visiblePositionForPoint(pt);

    VisibleSelection newSelection;
    newSelection = VisibleSelection(startPos);
    SelectionController newSelectionControler;
    newSelectionControler.setSelection(newSelection);


    if( (newSelectionControler.selection().visibleStart() == frameSelectionContrler->selection().visibleStart()) &&
            (newSelectionControler.selection().visibleEnd() == frameSelectionContrler->selection().visibleEnd()) )
        {
        result |= 0x1;
        }

    newSelectionControler.modify(SelectionController::AlterationMove, DirectionForward, DocumentBoundary);
    if( (newSelectionControler.selection().visibleStart() == frameSelectionContrler->selection().visibleStart()) &&
            (newSelectionControler.selection().visibleEnd() == frameSelectionContrler->selection().visibleEnd()) )
        {
        result |= 0x2;
        }

    return result;
}


void WebViewCore::saveSelectionController()
{
    ALOGV("VIN saveSelectionController called here");

    m_VisibleSelection = m_mainFrame->selection()->selection();
}

void WebViewCore::resetSelectionController()
{
    ALOGV("VIN resetSelectionController called here");

    m_VisibleSelection = VisibleSelection();
}

void WebViewCore::restorePreviousSelectionController()
{
    ALOGV("VIN restorePreviousSelectionController called here");

    SelectionController* selectionContrler = m_mainFrame->selection();

    if( m_VisibleSelection.isNonOrphanedCaretOrRange() ) {
        ALOGV("VIN restorePreviousSelectionController isNonOrphanedCaretOrRange() = true , isNone = %d, isCaret = %d, isRange = %d", m_VisibleSelection.isNone(), m_VisibleSelection.isCaret(), m_VisibleSelection.isRange());
        selectionContrler->setSelection(m_VisibleSelection);
    }

    m_VisibleSelection = VisibleSelection();
}

WTF::String WebViewCore::getBodyText()
{
    WTF::String plainText = m_mainFrame->document()->body()->innerText();
    plainText.replace(NonBreakingSpaceCharacter, SpaceCharacter);

    return plainText;
}


int WebViewCore::contentSelectionType()
{
    SelectionController* selectionContrler = m_mainFrame->selection();
    if(selectionContrler != NULL && !(selectionContrler->isNone()))
    {
        if (m_mainFrame->selection()->isRange())
            return 1;
        else
            return 0;
    }
    return -1;
}


bool WebViewCore::getBodyEmpty()
{
    bool retVal = false;
    Node* bodyFirstChild = m_mainFrame->document()->body()->firstChild();
    if(bodyFirstChild)
    {
        if( !bodyFirstChild->firstChild() && !bodyFirstChild->nextSibling() )
        {
            if(bodyFirstChild->isElementNode())
            {
                String tagname = bodyFirstChild->nodeName().lower();
                if(tagname == "br")
                {
                    retVal = true;
                }
            }
        }

    }
    else
    {
        retVal = true;
    }
    return retVal;
    /*bool retVal = false;
    unsigned childCount = m_mainFrame->document()->body()->childNodeCount();
    if(childCount > 1)
        return retVal;
    else if(childCount == 1)
    {
        Node* bodyFirstChild = m_mainFrame->document()->body()->firstChild();
        if(bodyFirstChild->isElementNode())
        {
            String tagname = bodyFirstChild->nodeName().lower();
            if(tagname == "br")
            {
                retVal = true;
            }
        }
    }
    else
        retVal = true;
    return retVal;*/
}

WTF::String WebViewCore::getBodyHTML()
{
    return m_mainFrame->document()->body()->outerHTML();
}

WebCore::IntRect WebViewCore::getCursorRect(bool giveContentRect)
{
    if( !m_mainFrame->selection()->isCaret() ) {
        WebCore::IntRect caretRect;

        caretRect.setX(-1);
        caretRect.setY(-1);
        caretRect.setWidth(0);
        caretRect.setHeight(0);

        return caretRect;
    }

    if(giveContentRect)
        return m_mainFrame->selection()->absoluteCaretBounds();
    else
    {
        WebCore::IntRect caretRect;
        caretRect = m_mainFrame->selection()->absoluteCaretBounds();//localCaretRect();
//        LOGD("getCursorRect %d %d %d %d " , caretRect.x() , caretRect.y() , caretRect.maxX() , caretRect.maxY());
        WebCore::IntPoint locInWindow = m_mainFrame->view()->contentsToWindow(caretRect.location());
        //caretRect = m_mainFrame->view()->convertToContainingView(caretRect);
        caretRect.setLocation(locInWindow);
 //       LOGD("getCursorRect %d %d %d %d " , caretRect.x() , caretRect.y() , caretRect.maxX() , caretRect.maxY());
        ALOGV("getCursorRect exit");
        return caretRect;
    }

}

void WebViewCore::setSelectionNone()
{
    m_mainFrame->selection()->setSelection(VisibleSelection());
}

bool WebViewCore::getSelectionNone()
{
      bool result = false;
      result = m_mainFrame->selection()->isCaret();
      return result;
}


void WebViewCore::setComposingSelectionNone()
{
      ALOGV("setComposingSelectionNone enter");
      m_mainFrame->editor()->confirmCompositionWithoutDisturbingSelection();
      m_mainFrame->editor()->setUnderLineUpdateFlag(false);
      //undelineVec.clear();
}

void WebViewCore::deleteSurroundingText(int left , int right)
{
    ALOGV("deleteSurroundingText enter");
    int cnt;

    WebCore::Frame* frame = focusedFrame();
    if(!frame || !frame->selection()) return;

    int startOffset = -1;
    int endOffset = -1;
    getSelectionOffset(startOffset , endOffset);

    if (startOffset > endOffset) {
        int tmp = startOffset;
        startOffset = endOffset;
        endOffset = tmp;
    }

    int deleted = 0;

    if (left > 0) {
        int start = startOffset - left;
        if (start < 0) start = 0;
        setSelectionEditable(startOffset, startOffset);

        simulateDelKeyForCount(left, true);

        deleted = startOffset - start;

        startOffset = start;
        endOffset = endOffset - deleted;

        updateIMSelection(startOffset, startOffset);
    }

    if (right > 0) {
        int end = endOffset + right;

        if (end > getBodyText().length()) end = getBodyText().length();
        setSelectionEditable(endOffset, endOffset);

        simulateDelKeyForCount(right, false);

        updateIMSelection(endOffset, endOffset);
    }

    setSelectionEditable( startOffset, endOffset );
    updateIMSelection(startOffset, endOffset);

    ALOGV("deleteSurroundingText exit");

}

void  WebViewCore::getSelectionOffsetImage()
{
    SelectionController* selectionContrler = m_mainFrame->selection();
    if(selectionContrler != NULL && !(selectionContrler->isNone()))
    {
        // Test code - selection of node using range 
        ExceptionCode ec = 0;

        //DOMSelection* selection = m_mainFrame->domWindow()->getSelection();
          
        PassRefPtr<Range> rangeRef = m_mainFrame->document()->createRange();
        rangeRef->selectNode(m_SelectedImageNode, ec);
        EAffinity affinity = selectionContrler->affinity();
        selectionContrler->setSelectedRange(rangeRef.get(), affinity, false);
    }
}


#define DEFAULT_OFFSET 0
void WebViewCore::getSelectionOffset(int& startOffset , int& endOffset)
{
    SelectionController* selectionContrler = m_mainFrame->selection();
    if(selectionContrler != NULL && !(selectionContrler->isNone()))
    {
            Position pos;
                  pos = selectionContrler->selection().visibleStart().deepEquivalent();

            IntPoint pt = IntPoint(0, 0);
            Position startPos = m_mainFrame->visiblePositionForPoint(pt).deepEquivalent();

            VisibleSelection newSelection;
            SelectionController newSelectionControler;
            //if (comparePositions(pos , startPos) <= 0)
                  //    newSelection = VisibleSelection(pos, startPos);
            //else
                        newSelection = VisibleSelection(startPos, pos);

            newSelectionControler.setSelection(newSelection);
            PassRefPtr<Range> rangePtr = newSelectionControler.toNormalizedRange();
            WebCore::Range* range = rangePtr.get();
            if(range != NULL)
            {
                  WTF::String plainText = range->text();
                  //plainText.replace(NonBreakingSpaceCharacter, SpaceCharacter);

                  //DEBUG_NAV_UI_LOGD("getSelectionOffset %s len %d" , plainText.utf8().data() , plainText.length());

                  //return plainText.length();
                  startOffset = plainText.length();
            if(selectionContrler->isRange())
            {
                endOffset = startOffset + m_mainFrame->editor()->selectedText().length();
            }
            else
            {
                endOffset = startOffset;
            }
            ALOGV("getSelectionOffset str %d end %d" , startOffset ,endOffset);
    }
      }


}


bool WebViewCore::execCommand(WTF::String& commandName ,  WTF::String& value)
{
    //ANDROID_LOG_PRINT(ANDROID_LOG_DEBUG, "HTML_EDIT", " WebViewCore.cpp ::execCommand command: dfgdsf sdgfgs:%s", commandName.utf8().data());
    //ALOGV("execCommand entered %s value %s " , commandName.utf8().data() , value.utf8().data());
    bool retval = false;
	WebCore::Frame* frame = NULL;
	if(m_mainFrame == focusedFrame()){
		frame = m_mainFrame;
	}else{
		frame = focusedFrame();
	}
	if(frame == NULL)
		return false;
	
    if(commandName == "MoveToBeginningOfDocument" || commandName == "MoveToEndOfDocument")
    {
		SelectionController* selectionContrler = frame->selection();
        if(selectionContrler->isNone())
        {
            ALOGV("execCommand selection none");
            //VisiblePosition vPos = VisiblePosition(m_mainFrame->document()->body() , 0 );
            Position startPos = Position(frame->document()->body() , 0 );//vPos.deepEquivalent();

            VisibleSelection newSelection;
            newSelection = VisibleSelection(startPos);
            selectionContrler->setSelection(newSelection);
        }
        else
            ALOGV("execCommand selection is Not none");
    }
    retval = frame->editor()->command(commandName).execute(value);

    if( frame->selection()->isRange() && commandName.find("ModifySelection") ) {
        frame->eventHandler()->setMouseDownMayStartSelect(true);
    }

    return retval;
}

bool WebViewCore::canUndo()
{
    return m_mainFrame->editor()->canUndo();
}

bool WebViewCore::canRedo()
{
    return m_mainFrame->editor()->canRedo();
}
void WebViewCore::undoRedoStateReset()
{
    return m_mainFrame->editor()->client()->clearUndoRedoOperations();
}


bool WebViewCore::copyAndSaveImage(WTF::String& imageUrl)
{
    WTF::String filePath;
    filePath = WebCore::createLocalResource(m_mainFrame , imageUrl);
    if(filePath.isEmpty())
        return false;

    WebCore::copyImagePathToClipboard(filePath);
    return true;
    //return false;
}

WebHTMLMarkupData* WebViewCore::getFullMarkupData(){
    //ANDROID_LOG_PRINT(ANDROID_LOG_DEBUG, "HTML_EDIT", " WebViewCore::getFullMarkupData() Called");

    setEditable(false);
    WebHTMLMarkupData* webMarkupData = WebCore::createFullMarkup(m_mainFrame->document() , "");
    setEditable(true);

    return webMarkupData;
}
void WebViewCore::setSelectionEditable(int start, int end)
{
    VisibleSelection retSel = setSelectionInVisibleSelection(start, end);
    if(!retSel.isNone())
    {
        m_mainFrame->selection()->setSelection(retSel);
    }
}

void WebViewCore::setComposingRegion(int start, int end)
{
    setComposingSelectionNone();
    VisibleSelection retSel = setSelectionInVisibleSelection(start, end);
    if(!retSel.isNone())
    {
            //m_composingVisibleSelection = retSel;
            VisibleSelection oldSelec = m_mainFrame->selection()->selection();
            m_mainFrame->selection()->setSelection(retSel);
            WTF::String selectedText = getSelectedText();
            int startOffset =-1;
            int endOffset = -1;
            Vector<CompositionUnderline> undVec;
            CompositionUnderline compositionDeco;
// selected text for setting composition should not contain the \n character so we are eleminating  this character when
// we are inserting selected text for composing
            if(selectedText.find('\n') != -1) {
                selectedText.replace('\n',"");
            }
            compositionDeco.startOffset = 0;
           compositionDeco.endOffset = selectedText.length();
            compositionDeco.isHighlightColor = false;
            undVec.append(compositionDeco);
            insertContent(selectedText , 1 , true , undVec, startOffset , endOffset);
            if(oldSelec.start().isCandidate())
            {
                  m_mainFrame->selection()->setSelection(oldSelec);
    }
      }
}
void WebViewCore::setPageZoomFact(float factor){
    if(factor > 0){
    float zoom_factor = (float)factor/100;
    m_mainFrame->setPageZoomFactor(zoom_factor);	
    }
}
VisibleSelection WebViewCore::setSelectionInVisibleSelection(int start, int end)
{

    int tempStart = start - m_imStr;
    int tempEnd = end - m_imEnd;

    ALOGV("setSelectionInVisibleSelection Enter - start = %d , end= %d  , m_imStr =%d , m_imEnd=%d " , start, end,m_imStr,m_imEnd);

    SelectionController* selectionContrler = m_mainFrame->selection();
    if(selectionContrler != NULL)
    {
        ALOGV("setSelectionInVisibleSelection Enter 1");

        if(selectionContrler->isNone())
            {
                ALOGV("setSelectionInVisibleSelection Enter 2");

            Position startPos = Position(m_mainFrame->document()->body() , 0 );//vPos.deepEquivalent();
            VisibleSelection newSelection;
                    newSelection = VisibleSelection(startPos);
                    selectionContrler->setSelection(newSelection);
            }
        SelectionController newSelectionControler;
        newSelectionControler.setSelection(selectionContrler->selection());

        if(newSelectionControler.isRange())
        {
            ALOGV("newSelectionControler.isRange() moving backward once");
                  newSelectionControler.modify(SelectionController::AlterationMove, DirectionBackward, CharacterGranularity);
        }

        int uTempStart  = tempStart >= 0 ? tempStart  : -tempStart;
        int cnt;
        VisibleSelection selectionToBackDelete, selectionToForwardDelete;

        ALOGV("setSelectionInVisibleSelection tempStart =%d ",tempStart);

        if(tempStart >= 0)
        {
            for(cnt = 0 ; cnt < uTempStart ; cnt++)
            {
                selectionToForwardDelete = newSelectionControler.selection();
                long deleteChar = (long)(selectionToForwardDelete.visibleEnd().characterAfter());
                if( deleteChar >= EMOJI_LOWER_VALUE && deleteChar <= EMOJI_UPPER_VALUE ) {
                    cnt++;
                }
                ALOGV("setSelectionInVisibleSelection (Move-forward) cnt =%d ",cnt);
                newSelectionControler.modify(SelectionController::AlterationMove, DirectionForward, CharacterGranularity);
            }
        }
        else
        {
            for(cnt = 0 ; cnt < uTempStart ; cnt++)
            {
                selectionToBackDelete = newSelectionControler.selection();
                long deleteChar = (long)(selectionToBackDelete.visibleStart().characterBefore());
                if( deleteChar >= EMOJI_LOWER_VALUE && deleteChar <= EMOJI_UPPER_VALUE ) {
                    cnt++;
                }
                ALOGV("setSelectionInVisibleSelection (Move-backward) cnt =%d ",cnt);
                newSelectionControler.modify(SelectionController::AlterationMove, DirectionBackward, CharacterGranularity);
            }
        }
        for(cnt = 0 ; cnt < (end - start) ; cnt++)
        {
            selectionToForwardDelete = newSelectionControler.selection();
            long deleteChar = (long)(selectionToForwardDelete.visibleEnd().characterAfter());
            if( deleteChar >= EMOJI_LOWER_VALUE && deleteChar <= EMOJI_UPPER_VALUE ) {
                cnt++;
            }
            ALOGV("setSelectionInVisibleSelection (extend forward) cnt =%d ",cnt);
            newSelectionControler.modify(SelectionController::AlterationExtend, DirectionForward, CharacterGranularity);
        }
        //selectionContrler->setSelection(newSelectionControler.selection());
        return newSelectionControler.selection();
    }
    ALOGV("setSelectionInVisibleSelection return VisibleSelection ");
    return VisibleSelection();
}


void WebViewCore::setEditable(bool enableEditing)
{
	ExceptionCode ec;
    if(enableEditing)
        m_mainFrame->document()->body()->setContentEditable("true", ec);
    else
        m_mainFrame->document()->body()->setContentEditable("false", ec);
}

// There is the case that only "\n" is selected or only " " is selected. 
// In the case of "\n", the shadow for Selection Area doesn't appear.
// So, it can be misunderstood as cursor is not displayed.
// In the case of " ", it is not the same concept as EditText.
// EditText doesn't support to select " " when selecting word in ICS.
// This function will check the status and display the cursor instead of Selection Area.
// And a selected " " will be ignored by it.
// jaesung.yi@samsung.com
void WebViewCore::checkSelectedClosestWord()
{
    int selectedType = -2;
    WTF::String selectedText = getSelectedText();

    selectedType = contentSelectionType();
	
    if( 1 == selectedType /*range*/ && NULL != selectedText 
        && ( selectedText == WTF::String("\n") || ( selectedText.length() == 1 && selectedText.contains(NonBreakingSpaceCharacter) ) ) )
    {
        SelectionController* selectionContrler = m_mainFrame->selection();

        if(selectionContrler != NULL)
        {
            Position startSelPos = selectionContrler->selection().visibleStart().deepEquivalent();

            selectionContrler->setSelection(VisibleSelection(startSelPos));

            selectionContrler->setCaretVisible(true);
        }
    }
}

bool WebViewCore::isEditableSupport()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();

    bool result = env->CallBooleanMethod(m_javaGlue->object(env).get(),
        m_javaGlue->m_isEditableSupport);

    checkException(env);
    return result;
}

bool WebViewCore::isDestroyPending()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();

    bool result = env->CallBooleanMethod(m_javaGlue->object(env).get(),
        m_javaGlue->m_isDestroyPending);

    checkException(env);
    return result;
}
bool WebViewCore::isTouchedOutside(int x,int y){
    IntPoint pt = IntPoint(x, y);
    WebCore::HitTestResult hitTestResult = m_mainFrame->eventHandler()->hitTestResultAtPoint(pt, true);
    WebCore::Node* node = hitTestResult.innerNode();

    if(node && node->isElementNode() && ( node->nodeName().lower() == "html" )) {
        __android_log_print(ANDROID_LOG_DEBUG,"WebviewCore", "isTouchedOutside");
        return true;
    }

    return false;
}


void WebViewCore::moveSingleCursorHandler(int x,int y){

    ALOGV("moveSingleCursorHandler Enter - x = %d , y= %d" , x, y);

    SelectionController* frameSelectionContrler = m_mainFrame->selection();
    if(frameSelectionContrler == NULL)
        return ;
//    y=y-20;
    IntPoint pt = IntPoint(x, y);
    WebCore::HitTestResult hitTestResult = m_mainFrame->eventHandler()->hitTestResultAtPoint(pt, true);
    WebCore::Node* node = hitTestResult.innerNode();

    if(node && node->isElementNode() && ( node->nodeName().lower() == "html" )) {
        __android_log_print(ANDROID_LOG_DEBUG,"WebviewCore", "moveSingleCursorHandler Outer Position");
        return;
    }

    VisiblePosition startPos = m_mainFrame->visiblePositionForPoint(pt);
    int correctionY = y;
    while ( node && node->isElementNode() && ( node->nodeName().lower() == "body" )) {
        correctionY -= 10;
        IntPoint pt1 = IntPoint(x, correctionY);
        if( 0 > pt1.y() ) return;    //invalid

        ALOGV("moveSingleCursorHandler pt1.x = [%d], pt1.y = [%d]", pt1.x(), pt1.y());

        hitTestResult = m_mainFrame->eventHandler()->hitTestResultAtPoint(pt1, true);
        node = hitTestResult.innerNode();

        if( !node ) return;
        RenderObject* renderer = node->renderer();
        if( !renderer) return;
        startPos = renderer->positionForPoint(hitTestResult.localPoint());
    }

    if( startPos.isNull() ) {
        __android_log_print(ANDROID_LOG_DEBUG,"WebviewCore", "moveSingleCursorHandler startPos == null");
        return;
    }

    VisibleSelection newSelection;
    newSelection = VisibleSelection(startPos);
    frameSelectionContrler->setSelection(newSelection/*(newSelectionControler.selection()*/);

    imageVisiblePosition = startPos;

    if(newSelection.start().isCandidate()) {
        imageCanMove= true;
    }
    else {
        imageCanMove= false;
    }

    ALOGV("moveSingleCursorHandler Leave ");

    return;
}



//+ by jaesung.yi 110802
int WebViewCore::getStateInRichlyEditableText()
{
    ALOGV("getStateInRichlyEditableText()"); 

    int totalResult = 0x0;
    int result = 0;

    result = m_mainFrame->editor()->command("Bold").state();

    if( TrueTriState == result ) {
        totalResult |= 0x0001;
    } else if( MixedTriState == result ) {
        totalResult |= 0x0002;
    }

    result = m_mainFrame->editor()->command("Italic").state();;

    if( TrueTriState == result ) {
        totalResult |= 0x0004;
    } else if( MixedTriState == result ) {
        totalResult |= 0x0008;
    }
/*
    result = m_mainFrame->editor()->command("Underline").state();

    if( TrueTriState == result ) {
        totalResult |= 0x0010;
    } else if( MixedTriState == result ) {
        totalResult |= 0x0020;
    }
*/
    result = m_mainFrame->editor()->command("InsertOrderedList").state();

    if( TrueTriState == result ) {
        totalResult |= 0x0040;
    } else if( MixedTriState == result ) {
        totalResult |= 0x0080;
    }

    result = m_mainFrame->editor()->command("InsertUnorderedList").state();

    if( TrueTriState == result ) {
        totalResult |= 0x0100;
    } else if( MixedTriState == result ) {
        totalResult |= 0x0200;
    }

    result = m_mainFrame->editor()->command("JustifyLeft").state();

    if( TrueTriState == result ) {
        totalResult |= 0x0400;
    } else if( MixedTriState == result ) {
        totalResult |= 0x0800;
    }

    result = m_mainFrame->editor()->command("JustifyCenter").state();

    if( TrueTriState == result ) {
        totalResult |= 0x1000;
    } else if( MixedTriState == result ) {
        totalResult |= 0x2000;
    }

    result = m_mainFrame->editor()->command("JustifyRight").state();

    if( TrueTriState == result ) {
        totalResult |= 0x4000;
    } else if( MixedTriState == result ) {
        totalResult |= 0x8000;
    }
    return totalResult;
}
//- by jaesung.yi 110802

// 0: etc, 1: The end of a word, 2: The boundary of the document or Image node. 
int WebViewCore::checkEndofWordAtPosition(int x, int y)
{
    //SAMSUNG CHANGE, yeonju.ann : Remove security logs
    //ALOGV("isTouchedPositionAtEndOfWord Enter - x = %d , y= %d" , x, y);

    SelectionController* frameSelectionContrler = m_mainFrame->selection();
    if(frameSelectionContrler == NULL)
        return 0;

    IntPoint pt = IntPoint(x, y);
    WebCore::HitTestResult hitTestResult = m_mainFrame->eventHandler()->hitTestResultAtPoint(pt, false, true);
    WebCore::Node* node = hitTestResult.innerNode();  

    if(node && node->isElementNode() && ( node->nodeName().lower() == "html" || node->nodeName().lower() == "img" )) {
        return 2;
    }

    VisiblePosition startPos = m_mainFrame->visiblePositionForPoint(pt);
    VisibleSelection newSelection;
    newSelection = VisibleSelection(startPos);

    SelectionController newSelectionControler;
    newSelectionControler.setSelection(newSelection);

    newSelectionControler.modify(SelectionController::AlterationExtend, DirectionForward, CharacterGranularity);

    PassRefPtr<Range> rangePtr = newSelectionControler.toNormalizedRange();
    WebCore::Range* range = rangePtr.get();
    if(range != NULL)
    {
        WTF::String plainText = range->text();
        if( 1 < plainText.length() ) {
            ALOGV("isTouchedPositionAtEndOfWord The selected character to check the end of word is more than one character. ");
            return 0;
        } else if( 0 == plainText.length() || ( 1 == plainText.length() && ( plainText.contains(NonBreakingSpaceCharacter) || plainText == WTF::String("\n") ) ) ) {
            ALOGV("isTouchedPositionAtEndOfWord A touched position is the end of a word.");
            frameSelectionContrler->setSelection(newSelection);
            return 1;
        }
    }

    return 0;
}

//+Feature_Support_SPen
WTF::String WebViewCore::getSelectedImageUri()
{
    return m_SelectedImageUri;
}
//-Feature_Support_SPen

//+Feature_SPen_Gesture_TextSelection
void WebViewCore::selectBWStartAndEnd( int startX, int startY, int endX, int endY )
{
    SelectionController* frameSelectionContrler = m_mainFrame->selection();
    if(frameSelectionContrler == NULL)
        return;

    VisiblePosition startPos = m_mainFrame->visiblePositionForPoint(IntPoint(startX, startY));
    VisiblePosition endPos = m_mainFrame->visiblePositionForPoint(IntPoint(endX, endY));

    VisibleSelection newSelection = VisibleSelection( startPos, endPos );

    SelectionController newSelectionController;
    newSelectionController.setSelection(newSelection);

    PassRefPtr<Range> rangePtr = newSelectionController.toNormalizedRange();
    WebCore::Range* range = rangePtr.get();
    if(range != NULL) {
        IntRect startRect, endRect;
        startRect = m_mainFrame->editor()->firstRectForRange(range);
        endRect = m_mainFrame->editor()->lastRectForRange(range);

//        __android_log_print(ANDROID_LOG_DEBUG,"WebviewCore", "selectBWStartAndEnd startRect x=[%d], y=[%d], maxX=[%d], maxY=[%d]", startRect.x(), startRect.y(), startRect.maxX(), startRect.maxY() );
//        __android_log_print(ANDROID_LOG_DEBUG,"WebviewCore", "selectBWStartAndEnd endRect x=[%d], y=[%d], maxX=[%d], maxY=[%d]", endRect.x(), endRect.y(), endRect.maxX(), endRect.maxY() );

        //if( startRect.maxY() != endRect.maxY() ) {
        if( (startRect.maxY() - endRect.maxY() > 5) || (endRect.maxY() - startRect.maxY() > 5) ) {
            __android_log_print(ANDROID_LOG_DEBUG,"WebviewCore", "selectBWStartAndEnd startRect and endRect are not on the same line");
            return;
        }

        WTF::String plainText = range->text();
        if( 0 < plainText.length() ) {
 //           __android_log_print(ANDROID_LOG_DEBUG,"WebviewCore", "selectBWStartAndEnd selected text = [%s]", plainText.utf8().data() );
            frameSelectionContrler->setSelection(newSelection);
            //SAMSUNG changes <S-PEN Text Selection>
            //Adding condition to bypass the startActionMode() call for HTMLComposer
            if(!(m_mainFrame->page()->settings()->editableSupportEnabled())) {
                startActionMode(true);
            }
            //SAMSUNG changes <S-PEN Text Selection>
            m_mainFrame->eventHandler()->setMouseDownMayStartSelect(true);
        } else {
            __android_log_print(ANDROID_LOG_DEBUG,"WebviewCore", "selectBWStartAndEnd selected text is a empty string" );
        }
    } else {
        __android_log_print(ANDROID_LOG_DEBUG,"WebviewCore", "selectBWStartAndEnd range == NULL" );
    }
}
//-Feature_SPen_Gesture_TextSelection

void WebViewCore::setCursorFromRangeSelectionController()
{
    ALOGV("VIN setCursorFromRangeSelectionController called here");

    SelectionController* selectionContrler = m_mainFrame->selection();
    Position endSelPos = selectionContrler->selection().visibleEnd().deepEquivalent();

    if(selectionContrler != NULL)
    {
        ALOGV("VIN selectionContrler->setSelection called here");
        selectionContrler->setSelection(VisibleSelection(endSelPos));

        WebCore::Node* focus = currentFocus();
        if (!focus) {
           ALOGV("!focus");
           return;
        }

        setFocusControllerActive(true);
    }
}

int WebViewCore::isAtBoundary(int x, int y)
{
    SelectionController* frameSelectionContrler = m_mainFrame->selection();
    if(frameSelectionContrler == NULL)
        return 0;

    IntPoint pt = IntPoint(x, y);
    WebCore::HitTestResult hitTestResult = m_mainFrame->eventHandler()->hitTestResultAtPoint(pt, false, true);
    WebCore::Node* node = hitTestResult.innerNode();

    if(node && node->isElementNode() && ( node->nodeName().lower() == "html" )) {
        return 1;
    }

    return 0;
}

//+Feature_Drag&Drop
void WebViewCore::dropTheDraggedText( int x, int y ) {
    IntPoint pt = IntPoint(x, y);
    VisiblePosition newStartPos = m_mainFrame->visiblePositionForPoint(pt);

    SelectionController* selectionContrler = m_mainFrame->selection();
//SISO_HTMLComposer start
    VisibleSelection previousRangeSelection = m_VisibleSelection;
    String textInRange;
//SISO_HTMLComposer end

    if( m_VisibleSelection.isNonOrphanedRange() ) {
        __android_log_print(ANDROID_LOG_DEBUG,"webcoreglue", "dropTheDraggedText isNonOrphanedRange() = %d , isNone = %d" , m_VisibleSelection.isNonOrphanedRange(), m_VisibleSelection.isNone());
        selectionContrler->setSelection(previousRangeSelection);
        m_VisibleSelection = VisibleSelection();
        textInRange = (m_mainFrame->selection()->toNormalizedRange())->text(); /* SISO_HTMLComposer */

        PassRefPtr<Range> rangePtr = previousRangeSelection.toNormalizedRange();
        WebCore::Range* range = rangePtr.get();
        WTF::Vector<IntRect> boxVector = range->boundingBoxEx();
        for(size_t i = 0; i<boxVector.size(); i++ ) {
            if( x >= boxVector[i].x() && x <= (boxVector[i].x()+boxVector[i].width() ) && y >= boxVector[i].y() && y <= (boxVector[i].y()+boxVector[i].height()) ) {
                return;
            } 
        }

        if(m_mainFrame == focusedFrame()){
            m_mainFrame->editor()->command("Delete").execute(NULL);
        }else{
            if( NULL == focusedFrame() ) return;
            focusedFrame()->editor()->command("Delete").execute(NULL);
        }
    }

    VisibleSelection newSelection;
    newSelection = VisibleSelection(newStartPos);
//SISO_HTMLComposer start
    if(newSelection.start().anchorNode() == previousRangeSelection.start().anchorNode() && textInRange.length() > 0) {
       int tempOffset = (newStartPos.deepEquivalent()).deprecatedEditingOffset() - textInRange.length();
       newSelection = VisibleSelection(Position(newSelection.start().anchorNode(),tempOffset),selectionContrler->affinity());
    }
//SISO_HTMLComposer end

    if( newSelection.isNone() ) return;    //invalid

    selectionContrler->setSelection(newSelection);
}
//-Feature_Drag&Drop

WTF::String WebViewCore::getSelectedHTMLText()
{
    SelectionController* selectionContrler = m_mainFrame->selection();

    if( !selectionContrler ) return WTF::String();

    PassRefPtr<Range> selectedRange = selectionContrler->toNormalizedRange();

    WTF::String html = createMarkup( selectedRange.get(), 0, AnnotateForInterchange, false );

    return html;
}

static WebCore::IntRect toContainingView(const WebCore::RenderObject* renderer, const WebCore::IntRect& rendererRect)
{
    WebCore::IntRect result = rendererRect;
    WebCore::RenderView *view = renderer->view() ;
    ALOGV("toContainingView: rendererRect(%d, %d, %d, %d)", result.x(), result.y(), result.width(), result.height());

    if (view && view->frameView() ) {
        WebCore::FrameView * frameView = view->frameView() ;
        if (const WebCore::ScrollView* parentScrollView = frameView->parent()) {
            if (parentScrollView->isFrameView()) {

                const FrameView* parentView = static_cast<const WebCore::FrameView*>(parentScrollView);

                // Get our renderer in the parent view
                WebCore::RenderPart* renderer = frameView->frame()->ownerRenderer();
                if (renderer) {
                    WebCore::IntPoint point(rendererRect.location());

                    // Add borders and padding
                    point.move(renderer->borderLeft() + renderer->paddingLeft(),
                        renderer->borderTop() + renderer->paddingTop());
                    WebCore::IntPoint pt = WebCore::roundedIntPoint(renderer->localToAbsolute(point, false, true /* use transforms */));
                    result.setLocation(pt);
                }

                //Let us verify the calculated location
                WebCore::IntPoint test(rendererRect.location());
                ALOGV("toContainingView: test(%d, %d)", test.x(), test.y());
                ScrollView* view = frameView;
                //while (view) {
                    ALOGV("toContainingView: frame position(%d, %d)", view->x(), view->y());
                    //test.move(view->x(), view->y());
                    //test = _convertToContainingWindow(frameView, test) ;
                    test = view->convertToContainingWindow(test);
                    //view = view->parent();
                //}
                IntPoint scroll ;
                while (view) {
                    scroll.move(view->scrollX(), view->scrollY());
                    view = view->parent();
                }
                test.move(scroll.x(), scroll.y()) ;

                if (test.x() > result.x() || test.y() > result.y()) {
                    ALOGV("toContainingView: Inconsistant result(%d, %d, %d, %d), recalculating using frame positions...", result.x(), result.y(), result.width(), result.height());
                    result.setLocation(test);
                }
            }
            else {
                result = frameView->Widget::convertToContainingView(result);
            }
        }
    }

    ALOGV("toContainingView: result(%d, %d, %d, %d)", result.x(), result.y(), result.width(), result.height());
    return result ;
}

WebCore::IntRect WebViewCore::getBlockBounds(WebCore::Node* node)
{
    WebCore::IntRect result;
    if (!node) {
        ALOGV("getRenderBlockBounds : HitTest Result Node is NULL!");
        return result;
    }
    WTF::String nodeName = node->nodeName() ;
    WTF::CString nodeNameLatin1 = nodeName.latin1() ;
    ALOGV("getRenderBlockBounds: node name = %s", nodeNameLatin1.data());


    WebCore::RenderObject *renderer = NULL ;
    WebCore::RenderObject* nodeRenderer = node->renderer();
    if (nodeRenderer != NULL) {
        ALOGV("getRenderBlockBounds: nodeRenderer = %s", nodeRenderer->renderName());
        if (nodeRenderer->isRenderPart()){
            renderer = nodeRenderer ;
        }
        else if (!nodeRenderer->isRenderBlock() && !nodeRenderer->isRenderImage()) {
            WebCore::RenderBlock *block = nodeRenderer->containingBlock() ;
            if (block) {
                renderer = block ;
            }
        }
        else {
            renderer = nodeRenderer ;
        }
    }
    else if (node->hasTagName(HTMLNames::areaTag) ){
        HTMLAreaElement *area = static_cast<HTMLAreaElement*>(node) ;

        if (area->shape() == HTMLAreaElement::Rect
            && node->parentNode()
            && node->parentNode()->hasTagName(HTMLNames::mapTag)) {

            Node *map = node->parentNode() ;
            if ( map->parentNode()) {
                WebCore::RenderObject *r = map->parentNode()->renderer() ;
                if (r->isRenderBlock()) {
                    IntRect parentRect = r->absoluteBoundingBoxRect() ;
                    result = area->rect() ;
                    result.move(parentRect.x(), parentRect.y()) ;

                    if (r->view() && r->view()->frameView())
                        result = toContainingView(r, result) ;
                }
            }
        }
    }

    if (renderer) {
        result = renderer->absoluteBoundingBoxRect() ;
        result = toContainingView(renderer, result) ;
    }

    if ( renderer == NULL)
      ALOGV("getRenderBlockBounds: No render block found!");
    else
        ALOGV("getRenderBlockBounds: node=%p result(%d, %d, %d, %d)", node, result.x(), result.y(), result.width(), result.height());

    return result;

}

WebCore::IntRect WebViewCore::getBlockBounds(const WebCore::IntPoint &pt)
{
    WebCore::IntRect result;
    ALOGV("getRenderBlockBounds: point=(%d, %d)", pt.x(), pt.y() );

    WebCore::HitTestResult hitTestResult = m_mainFrame->eventHandler()->hitTestResultAtPoint(pt, false, true);

    WebCore::Node* node = hitTestResult.innerNode();
    if (!node) {
        ALOGV("getRenderBlockBounds : HitTest Result Node is NULL!");
        return result;
    }else{
        return getBlockBounds(node);
    }
}

//SISO_HTMLCOMPOSER end

Position WebViewCore::getPositionForOffset(Node* node, int offset)
{
    Position start = firstPositionInNode(node);
    Position end = lastPositionInNode(node);
    Document* document = node->document();
    PassRefPtr<Range> range = Range::create(document, start, end);
    WebCore::CharacterIterator iterator(range.get());
    iterator.advance(offset);
    return iterator.range()->startPosition();
}

 void WebViewCore:: setSelection(WebCore::Node* node, int start)
 	{
 //	LOGI("called the funticon=%s",__FUNCTION__);
	node->document()->updateLayoutIgnorePendingStylesheets();
            Position  startPosition = getPositionForOffset(node, start);
       	
        VisibleSelection newSelection(startPosition, startPosition);
		if (Frame* frame = node->document()->frame())
			{
	//		LOGI("if condtion called called the funticon=%s",__FUNCTION__);
                            frame->selection()->setSelection(newSelection);
			}
 	}
void WebViewCore::setSelection(Node* node, int start, int end)
{
    RenderTextControl* control = toRenderTextControl(node);
    if (control)
        setSelectionRange(node, start, end);
    else {
        Position startPosition = getPositionForOffset(node, start);
        Position endPosition = getPositionForOffset(node, end);
        VisibleSelection selection(startPosition, endPosition);
        SelectionController* selector = node->document()->frame()->selection();
        selector->setSelection(selection);
    }
}

//SAMSUNG_THAI_EDITOR_FIX ++
void WebViewCore::setSelectionWithoutValidation(int start, int end)
{
	WebCore::Node* focus = currentFocus();
	if (!focus)
		return;
	if (start > end)
		swap(start, end);

	// Tell our EditorClient that this change was generated from the UI, so it
	// does not need to echo it to the UI.
	EditorClientAndroid* client = static_cast<EditorClientAndroid*>(
			m_mainFrame->editor()->client());
	client->setUiGeneratedSelectionChange(true);
	setSelection(focus, start, end);
	RenderTextControl* control = toRenderTextControl(focus);
	if (start != end && control) {
		// Fire a select event. No event is sent when the selection reduces to
		// an insertion point
		control->selectionChanged(true);
	}
    client->setUiGeneratedSelectionChange(false);
    WebCore::Frame* focusedFrame = focus->document()->frame();
    VisibleSelection selection = focusedFrame->selection()->selection();
    if (start != end 
        && selection.end().deprecatedEditingOffset() == selection.start().deprecatedEditingOffset()
        && selection.end().anchorNode() == selection.start().anchorNode()) {
        int e = selection.end().deprecatedEditingOffset() ;
        int s = e - (end - start) ;
        Position base(selection.end().anchorNode(), s) ;
        Position extent(selection.end().anchorNode(), e) ;
        if (!base.isNull() && !extent.isNull() && base != extent) {
            selection.setWithoutValidation(base, extent);
            focusedFrame->selection()->setSelection(selection);        
        }
    }
    bool isPasswordField = false;
    if (focus->isElementNode()) {
        WebCore::Element* element = static_cast<WebCore::Element*>(focus);
        if (WebCore::InputElement* inputElement = element->toInputElement())
    //SAMSUNG WML CHANGES >>
    {
    #if ENABLE(WML)
        if(focus->isWMLElement())
                    isPasswordField = static_cast<WebCore::WMLInputElement*>(inputElement)->isPasswordField();
        else
            isPasswordField = static_cast<WebCore::HTMLInputElement*>(inputElement)->isPasswordField();
    #endif
    }
    //SAMSUNG WML CHANGES <<
    }
    // For password fields, this is done in the UI side via
    // bringPointIntoView, since the UI does the drawing.
	if ((control && control->isTextArea()) || !isPasswordField)
        revealSelection();
}
//SAMSUNG_THAI_EDITOR_FIX --

void WebViewCore::setSelection(int start, int end)
{
    WebCore::Node* focus = currentFocus();
    if (!focus)
        return;
    if (start > end)
        swap(start, end);

    // Tell our EditorClient that this change was generated from the UI, so it
    // does not need to echo it to the UI.
    EditorClientAndroid* client = static_cast<EditorClientAndroid*>(
            m_mainFrame->editor()->client());
    client->setUiGeneratedSelectionChange(true);
    setSelection(focus, start, end);
    RenderTextControl* control = toRenderTextControl(focus);
    if (start != end && control) {
        // Fire a select event. No event is sent when the selection reduces to
        // an insertion point
        control->selectionChanged(true);
    }
    client->setUiGeneratedSelectionChange(false);
    bool isPasswordField = false;
    if (focus->isElementNode()) 
   {
        WebCore::Element* element = static_cast<WebCore::Element*>(focus);
        if (WebCore::InputElement* inputElement = element->toInputElement())
        {
         #if ENABLE(WML)
	        if(focus->isWMLElement())
                    isPasswordField = static_cast<WebCore::WMLInputElement*>(inputElement)->isPasswordField();
	        else
	            isPasswordField = static_cast<WebCore::HTMLInputElement*>(inputElement)->isPasswordField();
        #endif
        }
    }
    // For password fields, this is done in the UI side via
    // bringPointIntoView, since the UI does the drawing.
    if ((control && control->isTextArea()) || !isPasswordField)
        revealSelection();
}

String WebViewCore::modifySelection(const int direction, const int axis)
{
    DOMSelection* selection = m_mainFrame->domWindow()->getSelection();
    ASSERT(selection);
    // We've seen crashes where selection is null, but we don't know why
    // See http://b/5244036
    if (!selection)
        return String();
    if (selection->rangeCount() > 1)
        selection->removeAllRanges();
    switch (axis) {
        case AXIS_CHARACTER:
        case AXIS_WORD:
        case AXIS_SENTENCE:
            return modifySelectionTextNavigationAxis(selection, direction, axis);
        case AXIS_HEADING:
        case AXIS_SIBLING:
        case AXIS_PARENT_FIRST_CHILD:
        case AXIS_DOCUMENT:
            return modifySelectionDomNavigationAxis(selection, direction, axis);
        default:
            ALOGE("Invalid navigation axis: %d", axis);
            return String();
    }
}

void WebViewCore::scrollNodeIntoView(Frame* frame, Node* node)
{
    if (!frame || !node)
        return;

    Element* elementNode = 0;

    // If not an Element, find a visible predecessor
    // Element to scroll into view.
    if (!node->isElementNode()) {
        HTMLElement* body = frame->document()->body();
        do {
            if (node == body)
                return;
            node = node->parentNode();
        } while (node && !node->isElementNode() && !isVisible(node));
    }

    // Couldn't find a visible predecessor.
    if (!node)
        return;

    elementNode = static_cast<Element*>(node);
    elementNode->scrollIntoViewIfNeeded(true);
}

String WebViewCore::modifySelectionTextNavigationAxis(DOMSelection* selection, int direction, int axis)
{
    Node* body = m_mainFrame->document()->body();

    ExceptionCode ec = 0;
    String markup;

    // initialize the selection if necessary
    if (selection->rangeCount() == 0) {
        if (m_currentNodeDomNavigationAxis
                && validNode(m_mainFrame,
                m_mainFrame, m_currentNodeDomNavigationAxis)) {
            RefPtr<Range> rangeRef =
                selection->frame()->document()->createRange();
            rangeRef->selectNode(m_currentNodeDomNavigationAxis, ec);
            m_currentNodeDomNavigationAxis = 0;
            if (ec)
                return String();
            selection->addRange(rangeRef.get());
        } else if (currentFocus()) {
            selection->setPosition(currentFocus(), 0, ec);
        } else {
            selection->setPosition(body, 0, ec);
        }
        if (ec)
            return String();
    }

    // collapse the selection
    if (direction == DIRECTION_FORWARD)
        selection->collapseToEnd(ec);
    else
        selection->collapseToStart(ec);
    if (ec)
        return String();

    // Make sure the anchor node is a text node since we are generating
    // the markup of the selection which includes the anchor, the focus,
    // and any crossed nodes. Forcing the condition that the selection
    // starts and ends on text nodes guarantees symmetric selection markup.
    // Also this way the text content, rather its container, is highlighted.
    Node* anchorNode = selection->anchorNode();
    if (anchorNode->isElementNode()) {
        // Collapsed selection while moving forward points to the
        // next unvisited node and while moving backward to the
        // last visited node.
        if (direction == DIRECTION_FORWARD)
            advanceAnchorNode(selection, direction, markup, false, ec);
        else
            advanceAnchorNode(selection, direction, markup, true, ec);
        if (ec)
            return String();
        if (!markup.isEmpty())
            return markup;
    }

    // If the selection is at the end of a non white space text move
    // it to the next visible text node with non white space content.
    // This is a workaround for the selection getting stuck.
    anchorNode = selection->anchorNode();
    if (anchorNode->isTextNode()) {
        if (direction == DIRECTION_FORWARD) {
            String suffix = anchorNode->textContent().substring(
                    selection->anchorOffset(), caretMaxOffset(anchorNode));
            // If at the end of non white space text we advance the
            // anchor node to either an input element or non empty text.
            if (suffix.stripWhiteSpace().isEmpty()) {
                advanceAnchorNode(selection, direction, markup, true, ec);
            }
        } else {
            String prefix = anchorNode->textContent().substring(0,
                    selection->anchorOffset());
            // If at the end of non white space text we advance the
            // anchor node to either an input element or non empty text.
            if (prefix.stripWhiteSpace().isEmpty()) {
                advanceAnchorNode(selection, direction, markup, true, ec);
            }
        }
        if (ec)
            return String();
        if (!markup.isEmpty())
            return markup;
    }

    // extend the selection
    String directionStr;
    if (direction == DIRECTION_FORWARD)
        directionStr = "forward";
    else
        directionStr = "backward";

    String axisStr;
    if (axis == AXIS_CHARACTER)
        axisStr = "character";
    else if (axis == AXIS_WORD)
        axisStr = "word";
    else
        axisStr = "sentence";

    selection->modify("extend", directionStr, axisStr);

    // Make sure the focus node is a text node in order to have the
    // selection generate symmetric markup because the latter
    // includes all nodes crossed by the selection.  Also this way
    // the text content, rather its container, is highlighted.
    Node* focusNode = selection->focusNode();
    if (focusNode->isElementNode()) {
        focusNode = getImplicitBoundaryNode(selection->focusNode(),
                selection->focusOffset(), direction);
        if (!focusNode)
            return String();
        if (direction == DIRECTION_FORWARD) {
            focusNode = focusNode->traversePreviousSiblingPostOrder(body);
            if (focusNode && !isContentTextNode(focusNode)) {
                Node* textNode = traverseNextContentTextNode(focusNode,
                        anchorNode, DIRECTION_BACKWARD);
                if (textNode)
                    anchorNode = textNode;
            }
            if (focusNode && isContentTextNode(focusNode)) {
                selection->extend(focusNode, caretMaxOffset(focusNode), ec);
                if (ec)
                    return String();
            }
        } else {
            focusNode = focusNode->traverseNextSibling();
            if (focusNode && !isContentTextNode(focusNode)) {
                Node* textNode = traverseNextContentTextNode(focusNode,
                        anchorNode, DIRECTION_FORWARD);
                if (textNode)
                    anchorNode = textNode;
            }
            if (anchorNode && isContentTextNode(anchorNode)) {
                selection->extend(focusNode, 0, ec);
                if (ec)
                    return String();
            }
        }
    }

    // Enforce that the selection does not cross anchor boundaries. This is
    // a workaround for the asymmetric behavior of WebKit while crossing
    // anchors.
    anchorNode = getImplicitBoundaryNode(selection->anchorNode(),
            selection->anchorOffset(), direction);
    focusNode = getImplicitBoundaryNode(selection->focusNode(),
            selection->focusOffset(), direction);
    if (anchorNode && focusNode && anchorNode != focusNode) {
        Node* inputControl = getIntermediaryInputElement(anchorNode, focusNode,
                direction);
        if (inputControl) {
            if (direction == DIRECTION_FORWARD) {
                if (isDescendantOf(inputControl, anchorNode)) {
                    focusNode = inputControl;
                } else {
                    focusNode = inputControl->traversePreviousSiblingPostOrder(
                            body);
                    if (!focusNode)
                        focusNode = inputControl;
                }
                // We prefer a text node contained in the input element.
                if (!isContentTextNode(focusNode)) {
                    Node* textNode = traverseNextContentTextNode(focusNode,
                        anchorNode, DIRECTION_BACKWARD);
                    if (textNode)
                        focusNode = textNode;
                }
                // If we found text in the input select it.
                // Otherwise, select the input element itself.
                if (isContentTextNode(focusNode)) {
                    selection->extend(focusNode, caretMaxOffset(focusNode), ec);
                } else if (anchorNode != focusNode) {
                    // Note that the focusNode always has parent and that
                    // the offset can be one more that the index of the last
                    // element - this is how WebKit selects such elements.
                    selection->extend(focusNode->parentNode(),
                            focusNode->nodeIndex() + 1, ec);
                }
                if (ec)
                    return String();
            } else {
                if (isDescendantOf(inputControl, anchorNode)) {
                    focusNode = inputControl;
                } else {
                    focusNode = inputControl->traverseNextSibling();
                    if (!focusNode)
                        focusNode = inputControl;
                }
                // We prefer a text node contained in the input element.
                if (!isContentTextNode(focusNode)) {
                    Node* textNode = traverseNextContentTextNode(focusNode,
                            anchorNode, DIRECTION_FORWARD);
                    if (textNode)
                        focusNode = textNode;
                }
                // If we found text in the input select it.
                // Otherwise, select the input element itself.
                if (isContentTextNode(focusNode)) {
                    selection->extend(focusNode, caretMinOffset(focusNode), ec);
                } else if (anchorNode != focusNode) {
                    // Note that the focusNode always has parent and that
                    // the offset can be one more that the index of the last
                    // element - this is how WebKit selects such elements.
                    selection->extend(focusNode->parentNode(),
                            focusNode->nodeIndex() + 1, ec);
                }
                if (ec)
                   return String();
            }
        }
    }

    // make sure the selection is visible
    if (direction == DIRECTION_FORWARD)
        scrollNodeIntoView(m_mainFrame, selection->focusNode());
    else
        scrollNodeIntoView(m_mainFrame, selection->anchorNode());

    // format markup for the visible content
    RefPtr<Range> range = selection->getRangeAt(0, ec);
    if (ec)
        return String();
    IntRect bounds = range->boundingBox();
    selectAt(bounds.center().x(), bounds.center().y());
    markup = formatMarkup(selection);
    ALOGV("Selection markup: %s", markup.utf8().data());

    return markup;
}

Node* WebViewCore::getImplicitBoundaryNode(Node* node, unsigned offset, int direction)
{
    if (node->offsetInCharacters())
        return node;
    if (!node->hasChildNodes())
        return node;
    if (offset < node->childNodeCount())
        return node->childNode(offset);
    else
        if (direction == DIRECTION_FORWARD)
            return node->traverseNextSibling();
        else
            return node->traversePreviousNodePostOrder(
                    node->document()->body());
}

Node* WebViewCore::getNextAnchorNode(Node* anchorNode, bool ignoreFirstNode, int direction)
{
    Node* body = 0;
    Node* currentNode = 0;
    if (direction == DIRECTION_FORWARD) {
        if (ignoreFirstNode)
            currentNode = anchorNode->traverseNextNode(body);
        else
            currentNode = anchorNode;
    } else {
        body = anchorNode->document()->body();
        if (ignoreFirstNode)
            currentNode = anchorNode->traversePreviousSiblingPostOrder(body);
        else
            currentNode = anchorNode;
    }
    while (currentNode) {
        if (isContentTextNode(currentNode)
                || isContentInputElement(currentNode))
            return currentNode;
        if (direction == DIRECTION_FORWARD)
            currentNode = currentNode->traverseNextNode();
        else
            currentNode = currentNode->traversePreviousNodePostOrder(body);
    }
    return 0;
}

void WebViewCore::advanceAnchorNode(DOMSelection* selection, int direction,
        String& markup, bool ignoreFirstNode, ExceptionCode& ec)
{
    Node* anchorNode = getImplicitBoundaryNode(selection->anchorNode(),
            selection->anchorOffset(), direction);
    if (!anchorNode) {
        ec = NOT_FOUND_ERR;
        return;
    }
    // If the anchor offset is invalid i.e. the anchor node has no
    // child with that index getImplicitAnchorNode returns the next
    // logical node in the current direction. In such a case our
    // position in the DOM tree was has already been advanced,
    // therefore we there is no need to do that again.
    if (selection->anchorNode()->isElementNode()) {
        unsigned anchorOffset = selection->anchorOffset();
        unsigned childNodeCount = selection->anchorNode()->childNodeCount();
        if (anchorOffset >= childNodeCount)
            ignoreFirstNode = false;
    }
    // Find the next anchor node given our position in the DOM and
    // whether we want the current node to be considered as well.
    Node* nextAnchorNode = getNextAnchorNode(anchorNode, ignoreFirstNode,
            direction);
    if (!nextAnchorNode) {
        ec = NOT_FOUND_ERR;
        return;
    }
    if (nextAnchorNode->isElementNode()) {
        // If this is an input element tell the WebView thread
        // to set the cursor to that control.
        if (isContentInputElement(nextAnchorNode)) {
            IntRect bounds = nextAnchorNode->getRect();
            selectAt(bounds.center().x(), bounds.center().y());
        }
        Node* textNode = 0;
        // Treat the text content of links as any other text but
        // for the rest input elements select the control itself.
        if (nextAnchorNode->hasTagName(WebCore::HTMLNames::aTag))
            textNode = traverseNextContentTextNode(nextAnchorNode,
                    nextAnchorNode, direction);
        // We prefer to select the text content of the link if such,
        // otherwise just select the element itself.
        if (textNode) {
            nextAnchorNode = textNode;
        } else {
            if (direction == DIRECTION_FORWARD) {
                selection->setBaseAndExtent(nextAnchorNode,
                        caretMinOffset(nextAnchorNode), nextAnchorNode,
                        caretMaxOffset(nextAnchorNode), ec);
            } else {
                selection->setBaseAndExtent(nextAnchorNode,
                        caretMaxOffset(nextAnchorNode), nextAnchorNode,
                        caretMinOffset(nextAnchorNode), ec);
            }
            if (!ec)
                markup = formatMarkup(selection);
            // make sure the selection is visible
            scrollNodeIntoView(selection->frame(), nextAnchorNode);
            return;
        }
    }
    if (direction == DIRECTION_FORWARD)
        selection->setPosition(nextAnchorNode,
                caretMinOffset(nextAnchorNode), ec);
    else
        selection->setPosition(nextAnchorNode,
                caretMaxOffset(nextAnchorNode), ec);
}

bool WebViewCore::isContentInputElement(Node* node)
{
  return (isVisible(node)
          && (node->hasTagName(WebCore::HTMLNames::selectTag)
          || node->hasTagName(WebCore::HTMLNames::aTag)
          || node->hasTagName(WebCore::HTMLNames::inputTag)
          || node->hasTagName(WebCore::HTMLNames::buttonTag)));
}

bool WebViewCore::isContentTextNode(Node* node)
{
   if (!node || !node->isTextNode())
       return false;
   Text* textNode = static_cast<Text*>(node);
   return (isVisible(textNode) && textNode->length() > 0
       && !textNode->containsOnlyWhitespace());
}

Text* WebViewCore::traverseNextContentTextNode(Node* fromNode, Node* toNode, int direction)
{
    Node* currentNode = fromNode;
    do {
        if (direction == DIRECTION_FORWARD)
            currentNode = currentNode->traverseNextNode(toNode);
        else
            currentNode = currentNode->traversePreviousNodePostOrder(toNode);
    } while (currentNode && !isContentTextNode(currentNode));
    return static_cast<Text*>(currentNode);
}

Node* WebViewCore::getIntermediaryInputElement(Node* fromNode, Node* toNode, int direction)
{
    if (fromNode == toNode)
        return 0;
    if (direction == DIRECTION_FORWARD) {
        Node* currentNode = fromNode;
        while (currentNode && currentNode != toNode) {
            if (isContentInputElement(currentNode))
                return currentNode;
            currentNode = currentNode->traverseNextNodePostOrder();
        }
        currentNode = fromNode;
        while (currentNode && currentNode != toNode) {
            if (isContentInputElement(currentNode))
                return currentNode;
            currentNode = currentNode->traverseNextNode();
        }
    } else {
        Node* currentNode = fromNode->traversePreviousNode();
        while (currentNode && currentNode != toNode) {
            if (isContentInputElement(currentNode))
                return currentNode;
            currentNode = currentNode->traversePreviousNode();
        }
        currentNode = fromNode->traversePreviousNodePostOrder();
        while (currentNode && currentNode != toNode) {
            if (isContentInputElement(currentNode))
                return currentNode;
            currentNode = currentNode->traversePreviousNodePostOrder();
        }
    }
    return 0;
}

bool WebViewCore::isDescendantOf(Node* parent, Node* node)
{
    Node* currentNode = node;
    while (currentNode) {
        if (currentNode == parent) {
            return true;
        }
        currentNode = currentNode->parentNode();
    }
    return false;
}

String WebViewCore::modifySelectionDomNavigationAxis(DOMSelection* selection, int direction, int axis)
{
    HTMLElement* body = m_mainFrame->document()->body();
    if (!m_currentNodeDomNavigationAxis && selection->focusNode()) {
        m_currentNodeDomNavigationAxis = selection->focusNode();
        selection->empty();
        if (m_currentNodeDomNavigationAxis->isTextNode())
            m_currentNodeDomNavigationAxis =
                m_currentNodeDomNavigationAxis->parentNode();
    }
    if (!m_currentNodeDomNavigationAxis)
        m_currentNodeDomNavigationAxis = currentFocus();
    if (!m_currentNodeDomNavigationAxis
            || !validNode(m_mainFrame, m_mainFrame,
                                        m_currentNodeDomNavigationAxis))
        m_currentNodeDomNavigationAxis = body;
    Node* currentNode = m_currentNodeDomNavigationAxis;
    if (axis == AXIS_HEADING) {
        if (currentNode == body && direction == DIRECTION_BACKWARD)
            currentNode = currentNode->lastDescendant();
        do {
            if (direction == DIRECTION_FORWARD)
                currentNode = currentNode->traverseNextNode(body);
            else
                currentNode = currentNode->traversePreviousNode(body);
        } while (currentNode && (currentNode->isTextNode()
            || !isVisible(currentNode) || !isHeading(currentNode)));
    } else if (axis == AXIS_PARENT_FIRST_CHILD) {
        if (direction == DIRECTION_FORWARD) {
            currentNode = currentNode->firstChild();
            while (currentNode && (currentNode->isTextNode()
                    || !isVisible(currentNode)))
                currentNode = currentNode->nextSibling();
        } else {
            do {
                if (currentNode == body)
                    return String();
                currentNode = currentNode->parentNode();
            } while (currentNode && (currentNode->isTextNode()
                    || !isVisible(currentNode)));
        }
    } else if (axis == AXIS_SIBLING) {
        do {
            if (direction == DIRECTION_FORWARD)
                currentNode = currentNode->nextSibling();
            else {
                if (currentNode == body)
                    return String();
                currentNode = currentNode->previousSibling();
            }
        } while (currentNode && (currentNode->isTextNode()
                || !isVisible(currentNode)));
    } else if (axis == AXIS_DOCUMENT) {
        currentNode = body;
        if (direction == DIRECTION_FORWARD)
            currentNode = currentNode->lastDescendant();
    } else {
        ALOGE("Invalid axis: %d", axis);
        return String();
    }
    if (currentNode) {
        m_currentNodeDomNavigationAxis = currentNode;
        scrollNodeIntoView(m_mainFrame, currentNode);
        String selectionString = createMarkup(currentNode);
        ALOGV("Selection markup: %s", selectionString.utf8().data());
        return selectionString;
    }
    return String();
}

bool WebViewCore::isHeading(Node* node)
{
    if (node->hasTagName(WebCore::HTMLNames::h1Tag)
            || node->hasTagName(WebCore::HTMLNames::h2Tag)
            || node->hasTagName(WebCore::HTMLNames::h3Tag)
            || node->hasTagName(WebCore::HTMLNames::h4Tag)
            || node->hasTagName(WebCore::HTMLNames::h5Tag)
            || node->hasTagName(WebCore::HTMLNames::h6Tag)) {
        return true;
    }

    if (node->isElementNode()) {
        Element* element = static_cast<Element*>(node);
        String roleAttribute =
            element->getAttribute(WebCore::HTMLNames::roleAttr).string();
        if (equalIgnoringCase(roleAttribute, "heading"))
            return true;
    }

    return false;
}

bool WebViewCore::isVisible(Node* node)
{
    // start off an element
    Element* element = 0;
    if(!node)
	return false;
	
    if (node->isElementNode())
        element = static_cast<Element*>(node);
    else
        element = node->parentElement();

    if(!element	)
	return false;	
    // check renderer
    if (!element->renderer()) {
        return false;
    }
    // check size
    if (element->offsetHeight() == 0 || element->offsetWidth() == 0) {
        return false;
    }
    // check style
    Node* body = m_mainFrame->document()->body();
    Node* currentNode = element;
    while (currentNode && currentNode != body) {
        RenderStyle* style = currentNode->computedStyle();
        if (style &&
                (style->display() == WebCore::NONE || style->visibility() == WebCore::HIDDEN)) {
            return false;
        }
        currentNode = currentNode->parentNode();
    }
    return true;
}

String WebViewCore::formatMarkup(DOMSelection* selection)
{
    ExceptionCode ec = 0;
    String markup = String();
    RefPtr<Range> wholeRange = selection->getRangeAt(0, ec);
    if (ec)
        return String();
    if (!wholeRange->startContainer() || !wholeRange->startContainer())
        return String();
    // Since formatted markup contains invisible nodes it
    // is created from the concatenation of the visible fragments.
    Node* firstNode = wholeRange->firstNode();
    Node* pastLastNode = wholeRange->pastLastNode();
    Node* currentNode = firstNode;
    RefPtr<Range> currentRange;

    while (currentNode != pastLastNode) {
        Node* nextNode = currentNode->traverseNextNode();
        if (!isVisible(currentNode)) {
            if (currentRange) {
                markup = markup + currentRange->toHTML().utf8().data();
                currentRange = 0;
            }
        } else {
            if (!currentRange) {
                currentRange = selection->frame()->document()->createRange();
                if (ec)
                    break;
                if (currentNode == firstNode) {
                    currentRange->setStart(wholeRange->startContainer(),
                        wholeRange->startOffset(), ec);
                    if (ec)
                        break;
                } else {
                    currentRange->setStart(currentNode->parentNode(),
                        currentNode->nodeIndex(), ec);
                    if (ec)
                       break;
                }
            }
            if (nextNode == pastLastNode) {
                currentRange->setEnd(wholeRange->endContainer(),
                    wholeRange->endOffset(), ec);
                if (ec)
                    break;
                markup = markup + currentRange->toHTML().utf8().data();
            } else {
                if (currentNode->offsetInCharacters())
                    currentRange->setEnd(currentNode,
                        currentNode->maxCharacterOffset(), ec);
                else
                    currentRange->setEnd(currentNode->parentNode(),
                            currentNode->nodeIndex() + 1, ec);
                if (ec)
                    break;
            }
        }
        currentNode = nextNode;
    }
    return markup.stripWhiteSpace();
}

void WebViewCore::selectAt(int x, int y)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_selectAt, x, y);
    checkException(env);
}

void WebViewCore::deleteSelection(int start, int end, int textGeneration)
{
    setSelection(start, end);
    if (start == end)
        return;
    WebCore::Node* focus = currentFocus();
    if (!focus)
        return;
    // Prevent our editor client from passing a message to change the
    // selection.
    EditorClientAndroid* client = static_cast<EditorClientAndroid*>(
            m_mainFrame->editor()->client());
    client->setUiGeneratedSelectionChange(true);
    PlatformKeyboardEvent down(AKEYCODE_DEL, 0, 0, true, false, false, false);
    PlatformKeyboardEvent up(AKEYCODE_DEL, 0, 0, false, false, false, false);
    key(down);
    key(up);
    client->setUiGeneratedSelectionChange(false);
    m_textGeneration = textGeneration;
}

void WebViewCore::replaceTextfieldText(int oldStart,
        int oldEnd, const WTF::String& replace, int start, int end,
        int textGeneration)
{
    WebCore::Node* focus = currentFocus();
    if (!focus)
        return;
	if (isDateTime(focus))
	{
	    setSelection(oldStart, oldEnd);
	}
	else
	{
	    setSelectionWithoutValidation(oldStart, oldEnd);
	}
    // Prevent our editor client from passing a message to change the
    // selection.
    EditorClientAndroid* client = static_cast<EditorClientAndroid*>(
            m_mainFrame->editor()->client());
    client->setUiGeneratedSelectionChange(true);
    if (replace.length())
        WebCore::TypingCommand::insertText(focus->document(), replace,
                false);
    else
        WebCore::TypingCommand::deleteSelection(focus->document());
    client->setUiGeneratedSelectionChange(false);
    // setSelection calls revealSelection, so there is no need to do it here.
    setSelection(start, end);
    m_textGeneration = textGeneration;
}

void WebViewCore::passToJs(int generation, const WTF::String& current,
    const PlatformKeyboardEvent& event)
{
    WebCore::Node* focus = currentFocus();
    if (!focus) {
        clearTextEntry();
        return;
    }
    // Block text field updates during a key press.
    m_blockTextfieldUpdates = true;
    // Also prevent our editor client from passing a message to change the
    // selection.
    EditorClientAndroid* client = static_cast<EditorClientAndroid*>(
            m_mainFrame->editor()->client());
    client->setUiGeneratedSelectionChange(true);
    key(event);
    client->setUiGeneratedSelectionChange(false);
    m_blockTextfieldUpdates = false;
    m_textGeneration = generation;
    WTF::String test = getInputText(focus);
    if (test != current) {
        // If the text changed during the key event, update the UI text field.
        updateTextfield(focus, test);//4.2 Merge
    }
    // Now that the selection has settled down, send it.
    updateTextSelection();
}

void WebViewCore::scrollFocusedTextInput(float xPercent, int y)//4.2 Merge
{
    WebCore::Node* focus = currentFocus();
    if (!focus) {
        clearTextEntry();
        return;//4.2 Merge
    }
    WebCore::RenderTextControl* renderText = toRenderTextControl(focus);
    if (!renderText) {
        clearTextEntry();
        return;//4.2 Merge
    }

    int x = (int) (xPercent * (renderText->scrollWidth() -
//SAMSUNG CHANGES: MPSG100006060, MPSG100005931
        //WAS: renderText->clientWidth()));
        renderText->contentWidth())); //Use content width instead of client width to calculate scroll
//SAMSUNG CHANGES <<
    renderText->setScrollLeft(x);
    renderText->setScrollTop(y);
    focus->document()->frame()->selection()->recomputeCaretRect();
    updateTextSelection();
//4.2 Merge BEGIN <<
//Follwong code replaced by     updateTextSelection();
//    LayerAndroid* layer = 0;
//    platformLayerIdFromNode(focus, &layer);
//    return absoluteContentRect(focus, layer);
// 4.2 Merge END >>
}

void WebViewCore::setFocusControllerActive(bool active)
{
    m_mainFrame->page()->focusController()->setActive(active);
}

void WebViewCore::saveDocumentState(WebCore::Frame* frame)
{
    if (!validNode(m_mainFrame, frame, 0))
        frame = m_mainFrame;
    WebCore::HistoryItem *item = frame->loader()->history()->currentItem();

    // item can be null when there is no offical URL for the current page. This happens
    // when the content is loaded using with WebCoreFrameBridge::LoadData() and there
    // is no failing URL (common case is when content is loaded using data: scheme)
    if (item) {
        item->setDocumentState(frame->document()->formElementsState());
    }
}

// Create an array of java Strings.
static jobjectArray makeLabelArray(JNIEnv* env, const uint16_t** labels, size_t count)
{
    jclass stringClass = env->FindClass("java/lang/String");
    ALOG_ASSERT(stringClass, "Could not find java/lang/String");
    jobjectArray array = env->NewObjectArray(count, stringClass, 0);
    ALOG_ASSERT(array, "Could not create new string array");

    for (size_t i = 0; i < count; i++) {
        jobject newString = env->NewString(&labels[i][1], labels[i][0]);
        env->SetObjectArrayElement(array, i, newString);
        env->DeleteLocalRef(newString);
        checkException(env);
    }
    env->DeleteLocalRef(stringClass);
    return array;
}

void WebViewCore::openFileChooser(PassRefPtr<WebCore::FileChooser> chooser)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;

    if (!chooser)
        return;

    WTF::String acceptType = chooser->acceptTypes();
    WTF::String capture;

#if ENABLE(MEDIA_CAPTURE)
    capture = chooser->capture();
#endif

    jstring jAcceptType = wtfStringToJstring(env, acceptType, true);
    jstring jCapture = wtfStringToJstring(env, capture, true);
    jstring jName = (jstring) env->CallObjectMethod(
            javaObject.get(), m_javaGlue->m_openFileChooser, jAcceptType, jCapture);
    checkException(env);
    env->DeleteLocalRef(jAcceptType);
    env->DeleteLocalRef(jCapture);

    WTF::String wtfString = jstringToWtfString(env, jName);
    env->DeleteLocalRef(jName);

    if (!wtfString.isEmpty()) {
	// SAMSUNG CHANGE + need to escape special characters as they are in encoded form, otherwise file open will fail.
        std::string filename = wtfString.utf8().data();
        if (filename.find("file://") == 0) {// starts with file://
            wtfString = decodeURLEscapeSequences(wtfString);
            //DBG_NAV_LOG("file: escaped");
    	}
	// SAMSUNG CHANGE -
        chooser->chooseFile(wtfString);
    }
}

//SAMSUNG CHANGE HTML5 COLOR <<
void WebViewCore::openColorChooser(WebCore::ColorChooserClientAndroid* chooserclient)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;

    if (m_colorChooser != 0)
        return;

    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_openColorChooser);
    checkException(env);

    m_colorChooser = chooserclient;

}
//SAMSUNG CHANGE HTML5 COLOR <<

//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES <<
void WebViewCore::requestDateTimePickers(const WTF::String& type , const WTF::String& value)
{
    ALOG_ASSERT(m_javaGlue->m_obj, "A Java widget was not associated with this view bridge!");

    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    jstring jinputStr = wtfStringToJstring(env, type);	
    jstring jvalueStr = NULL;
    if(value != NULL)
        jvalueStr = wtfStringToJstring(env, value);	
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_requestDateTimePickers,jinputStr, jvalueStr);
    checkException(env);
}

//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES >>

void WebViewCore::listBoxRequest(WebCoreReply* reply, const uint16_t** labels, size_t count, const int enabled[], size_t enabledCount,
        bool multiple, const int selected[], size_t selectedCountOrSelection)
{
    ALOG_ASSERT(m_javaGlue->m_obj, "No java widget associated with this view!");

    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;

    // If m_popupReply is not null, then we already have a list showing.
    if (m_popupReply != 0)
        return;

    //SAMSUNG_CHANGE Form navigation >>
    WebCore::Node* focusNode = currentFocus();
    if(isSelectInput(focusNode)) {
        initSelectField(focusNode);
    } else {
        ALOGD("Focused Node is not Select Input. Focused node name is <%s> and tag name is <%s>", 
            focusNode->nodeName().utf8().data(), 
            ((Element*) focusNode)->tagName().utf8().data());
        return;
    }
    //SAMSUNG_CHANGE Form navigation <<
    // Create an array of java Strings for the drop down.
    jobjectArray labelArray = makeLabelArray(env, labels, count);

    // Create an array determining whether each item is enabled.
    jintArray enabledArray = env->NewIntArray(enabledCount);
    checkException(env);
    jint* ptrArray = env->GetIntArrayElements(enabledArray, 0);
    checkException(env);
    for (size_t i = 0; i < enabledCount; i++) {
        ptrArray[i] = enabled[i];
    }
    env->ReleaseIntArrayElements(enabledArray, ptrArray, 0);
    checkException(env);

    if (multiple) {
        // Pass up an array representing which items are selected.
        jintArray selectedArray = env->NewIntArray(selectedCountOrSelection);
        checkException(env);
        jint* selArray = env->GetIntArrayElements(selectedArray, 0);
        checkException(env);
        for (size_t i = 0; i < selectedCountOrSelection; i++) {
            selArray[i] = selected[i];
        }
        env->ReleaseIntArrayElements(selectedArray, selArray, 0);

        env->CallVoidMethod(javaObject.get(),
                m_javaGlue->m_requestListBox, labelArray, enabledArray,
                selectedArray);
        env->DeleteLocalRef(selectedArray);
    } else {
        // Pass up the single selection.
        env->CallVoidMethod(javaObject.get(),
                m_javaGlue->m_requestSingleListBox, labelArray, enabledArray,
                selectedCountOrSelection);
    }

    env->DeleteLocalRef(labelArray);
    env->DeleteLocalRef(enabledArray);
    checkException(env);

    Retain(reply);
    m_popupReply = reply;
}

bool WebViewCore::key(const PlatformKeyboardEvent& event)
{
    WebCore::EventHandler* eventHandler;
    WebCore::Node* focusNode = currentFocus();
    if (focusNode) {
        WebCore::Frame* frame = focusNode->document()->frame();
        eventHandler = frame->eventHandler();
        VisibleSelection old = frame->selection()->selection();
        EditorClientAndroid* client = static_cast<EditorClientAndroid*>(
                m_mainFrame->editor()->client());
        client->setUiGeneratedSelectionChange(true);
        bool handled = eventHandler->keyEvent(event);
        client->setUiGeneratedSelectionChange(false);
        if (isContentEditable(focusNode)) {
            // keyEvent will return true even if the contentEditable did not
            // change its selection.  In the case that it does not, we want to
            // return false so that the key will be sent back to our navigation
            // system.
            handled |= frame->selection()->selection() != old;
        }
        //SAMSUNG_SHANGE [MPSG100005849] [P120801-5124] ++
        else if (isSelectInput(focusNode)) {
            if (AKEYCODE_ENTER == event.nativeVirtualKeyCode()) { 
                if(PlatformKeyboardEvent::KeyUp == event.type()) {
                    RenderMenuList* menuList = toRenderMenuList(focusNode->renderer());
                    menuList->showPopup();      
                }
                handled = true;
            }
        }
        //SAMSUNG_SHANGE [MPSG100005849] [P120801-5124] --
        return handled;
    } else {
        eventHandler = focusedFrame()->eventHandler();
    }
    return eventHandler->keyEvent(event);
}

bool WebViewCore::chromeCanTakeFocus(FocusDirection direction)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return false;
    return env->CallBooleanMethod(javaObject.get(), m_javaGlue->m_chromeCanTakeFocus, direction);
}

void WebViewCore::chromeTakeFocus(FocusDirection direction)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_chromeTakeFocus, direction);
}

void WebViewCore::setInitialFocus(const WebCore::PlatformKeyboardEvent& platformEvent)
{
    Frame* frame = focusedFrame();
    Document* document = frame->document();
    if (document)
        document->setFocusedNode(0);
    FocusDirection direction;
    switch (platformEvent.nativeVirtualKeyCode()) {
    case AKEYCODE_DPAD_LEFT:
        direction = FocusDirectionLeft;
        break;
    case AKEYCODE_DPAD_RIGHT:
        direction = FocusDirectionRight;
        break;
    case AKEYCODE_DPAD_UP:
        direction = FocusDirectionUp;
        break;
    default:
        direction = FocusDirectionDown;
        break;
    }
    RefPtr<KeyboardEvent> webkitEvent = KeyboardEvent::create(platformEvent, 0);
    m_mainFrame->page()->focusController()->setInitialFocus(direction,
            webkitEvent.get());
}

#if USE(ACCELERATED_COMPOSITING)
GraphicsLayerAndroid* WebViewCore::graphicsRootLayer() const
{
    RenderView* contentRenderer = m_mainFrame->contentRenderer();
    if (!contentRenderer)
        return 0;
    return static_cast<GraphicsLayerAndroid*>(
          contentRenderer->compositor()->rootPlatformLayer());
}
#endif

int WebViewCore::handleTouchEvent(int action, Vector<int>& ids, Vector<IntPoint>& points, int actionIndex, int metaState)
{
    int flags = 0;

#if USE(ACCELERATED_COMPOSITING)
    GraphicsLayerAndroid* rootLayer = graphicsRootLayer();
    if (rootLayer)
      rootLayer->pauseDisplay(true);
#endif

#if ENABLE(TOUCH_EVENTS) // Android
    #define MOTION_EVENT_ACTION_POINTER_DOWN 5
    #define MOTION_EVENT_ACTION_POINTER_UP 6

    WebCore::TouchEventType type = WebCore::TouchStart;
    WebCore::PlatformTouchPoint::State defaultTouchState;
    Vector<WebCore::PlatformTouchPoint::State> touchStates(points.size());

    switch (action) {
    case 0: // MotionEvent.ACTION_DOWN
        type = WebCore::TouchStart;
        defaultTouchState = WebCore::PlatformTouchPoint::TouchPressed;
        break;
    case 1: // MotionEvent.ACTION_UP
        type = WebCore::TouchEnd;
        defaultTouchState = WebCore::PlatformTouchPoint::TouchReleased;
        break;
    case 2: // MotionEvent.ACTION_MOVE
        type = WebCore::TouchMove;
        defaultTouchState = WebCore::PlatformTouchPoint::TouchMoved;
        break;
    case 3: // MotionEvent.ACTION_CANCEL
        type = WebCore::TouchCancel;
        defaultTouchState = WebCore::PlatformTouchPoint::TouchCancelled;
        break;
    case 5: // MotionEvent.ACTION_POINTER_DOWN
        type = WebCore::TouchStart;
        defaultTouchState = WebCore::PlatformTouchPoint::TouchStationary;
        break;
    case 6: // MotionEvent.ACTION_POINTER_UP
        type = WebCore::TouchEnd;
        defaultTouchState = WebCore::PlatformTouchPoint::TouchStationary;
        break;
    default:
        // We do not support other kinds of touch event inside WebCore
        // at the moment.
        ALOGW("Java passed a touch event type that we do not support in WebCore: %d", action);
        return 0;
    }

    for (int c = 0; c < static_cast<int>(points.size()); c++) {
        points[c].setX(points[c].x() - m_scrollOffsetX);
        points[c].setY(points[c].y() - m_scrollOffsetY);

        // Setting the touch state for each point.
        // Note: actionIndex will be 0 for all actions that are not ACTION_POINTER_DOWN/UP.
        if (action == MOTION_EVENT_ACTION_POINTER_DOWN && c == actionIndex) {
            touchStates[c] = WebCore::PlatformTouchPoint::TouchPressed;
        } else if (action == MOTION_EVENT_ACTION_POINTER_UP && c == actionIndex) {
            touchStates[c] = WebCore::PlatformTouchPoint::TouchReleased;
        } else {
            touchStates[c] = defaultTouchState;
        };
    }

    WebCore::PlatformTouchEvent te(ids, points, type, touchStates, metaState);
    //Was if (m_mainFrame->eventHandler()->handleTouchEvent(te))
	if (m_mainFrame  &&  m_mainFrame->eventHandler() && m_mainFrame->eventHandler()->handleTouchEvent(te)) // Null Pointer access shown in the logs so check added
        flags |= TOUCH_FLAG_PREVENT_DEFAULT;
    if (te.hitTouchHandler())
        flags |= TOUCH_FLAG_HIT_HANDLER;
#endif

#if USE(ACCELERATED_COMPOSITING)
    if (rootLayer)
      rootLayer->pauseDisplay(false);
#endif
    return flags;
}

bool WebViewCore::performMouseClick()
{
    WebCore::PlatformMouseEvent mouseDown(m_mouseClickPos, m_mouseClickPos, WebCore::LeftButton,
            WebCore::MouseEventPressed, 1, false, false, false, false,
            WTF::currentTime());
    // ignore the return from as it will return true if the hit point can trigger selection change
    m_mainFrame->eventHandler()->handleMousePressEvent(mouseDown);
    WebCore::PlatformMouseEvent mouseUp(m_mouseClickPos, m_mouseClickPos, WebCore::LeftButton,
            WebCore::MouseEventReleased, 1, false, false, false, false,
            WTF::currentTime());
    bool handled = m_mainFrame->eventHandler()->handleMouseReleaseEvent(mouseUp);

    WebCore::Node* focusNode = currentFocus();

//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES >>
   if(isDateTime(focusNode))
   {
	 WebCore::Element* ele = static_cast<Element*>(focusNode);
	 const AtomicString &typestr = ele->getAttribute(HTMLNames::typeAttr);
	 const WTF::String& typestring = typestr.string();
         const AtomicString &valuestr = ele->getAttribute(HTMLNames::valueAttr);
	 WebCore::RenderTextControl* rtc = toRenderTextControl(focusNode);		        	
         if(rtc){//SAMSUNG CHANGE -for prevent issue
	     const WTF::String& text = rtc->text();
	     if(!text){	
	         if((valuestr.isNull()))			    		                   
		    requestDateTimePickers(typestring,"");
	         else{
		    const WTF::String& valuestring = valuestr.string();
		    requestDateTimePickers(typestring,valuestring);	
	         }
	    }
	    else{
                requestDateTimePickers(typestring,text);	
	    }						
        }
    }//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES <<
    else {
        initializeTextInput(focusNode, false);
    }
    return handled;
}

// Check for the "x-webkit-soft-keyboard" attribute.  If it is there and
// set to hidden, do not show the soft keyboard.  Node passed as a parameter
// must not be null.
static bool shouldSuppressKeyboard(const WebCore::Node* node) {
    ALOG_ASSERT(node, "node passed to shouldSuppressKeyboard cannot be null");
    const NamedNodeMap* attributes = node->attributes();
    if (!attributes) return false;
    size_t length = attributes->length();
    for (size_t i = 0; i < length; i++) {
        const Attribute* a = attributes->attributeItem(i);
        if (a->localName() == "x-webkit-soft-keyboard" && a->value() == "hidden")
            return true;
    }
    return false;
}

WebViewCore::InputType WebViewCore::getInputType(Node* node)
{
    WebCore::RenderObject* renderer = node->renderer();
    if (!renderer)
        return WebViewCore::NONE;
    if (renderer->isTextArea())
        return WebViewCore::TEXT_AREA;

    if (node->hasTagName(WebCore::HTMLNames::inputTag)) {
        HTMLInputElement* htmlInput = static_cast<HTMLInputElement*>(node);
        if (htmlInput->isPasswordField())
            return WebViewCore::PASSWORD;
        if (htmlInput->isSearchField())
            return WebViewCore::SEARCH;
        if (htmlInput->isEmailField())
            return WebViewCore::EMAIL;
        if (htmlInput->isNumberField())
            return WebViewCore::NUMBER;
        if (htmlInput->isTelephoneField())
            return WebViewCore::TELEPHONE;
        //SISO CHANGE [MPSG100006079] ++
        if(htmlInput->isURLField())
            return WebViewCore::URL;
        //SISO CHANGE [MPSG100006079] --
        if (htmlInput->isTextField())
            return WebViewCore::NORMAL_TEXT_FIELD;
    }

    if (node->isContentEditable())
        return WebViewCore::TEXT_AREA;

    return WebViewCore::NONE;
}

int WebViewCore::getMaxLength(Node* node)
{
    int maxLength = -1;
    if (node->hasTagName(WebCore::HTMLNames::inputTag)) {
        HTMLInputElement* htmlInput = static_cast<HTMLInputElement*>(node);
        maxLength = htmlInput->maxLength();
    }
    return maxLength;
}

String WebViewCore::getFieldName(Node* node)
{
    String name;
    if (node->hasTagName(WebCore::HTMLNames::inputTag)) {
        HTMLInputElement* htmlInput = static_cast<HTMLInputElement*>(node);
        name = htmlInput->name();
    }
    return name;
}

bool WebViewCore::isSpellCheckEnabled(Node* node)
{
    bool isEnabled = true;
    if (node->isElementNode()) {
        WebCore::Element* element = static_cast<WebCore::Element*>(node);
        isEnabled = element->isSpellCheckingEnabled();
    }
    return isEnabled;
}

bool WebViewCore::isAutoCompleteEnabled(Node* node)
{
    bool isEnabled = false;
    if (node->hasTagName(WebCore::HTMLNames::inputTag)) {
        HTMLInputElement* htmlInput = static_cast<HTMLInputElement*>(node);
        isEnabled = htmlInput->autoComplete();
    }
    return isEnabled;
}

WebCore::IntRect WebViewCore::absoluteClientRect(WebCore::Node* node,
        LayerAndroid* layer)//4.2 Merge changed absoluteContentRect to absoluteClientRect
{
    IntRect clientRect;//4.2 Merge
    if (node) {
        RenderObject* render = node->renderer();
        if (render && render->isBox() && !render->isBody()) {
            IntPoint offset = convertGlobalContentToFrameContent(IntPoint(),
                    node->document()->frame());
            WebViewCore::layerToAbsoluteOffset(layer, offset);

            RenderBox* renderBox = toRenderBox(render);
// 4.2 Merge BEGIN <<
            clientRect = renderBox->clientBoxRect();
            IntRect contentBox = renderBox->contentBoxRect();
            clientRect.setX(contentBox.x());
            clientRect.setWidth(contentBox.width());
            FloatPoint absPos = renderBox->localToAbsolute(FloatPoint());
            clientRect.move(absPos.x() - offset.x(), absPos.y() - offset.y());
// 4.2 Merge END >>
        }
    }
    return clientRect;//4.2 Merge
}

jobject WebViewCore::createTextFieldInitData(Node* node)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    TextFieldInitDataGlue* classDef = m_textFieldInitDataGlue;
    ScopedLocalRef<jclass> clazz(env,
            env->FindClass("android/webkit/WebViewCore$TextFieldInitData"));
    jobject initData = env->NewObject(clazz.get(), classDef->m_constructor);
    env->SetIntField(initData, classDef->m_fieldPointer,
            reinterpret_cast<int>(node));
    ScopedLocalRef<jstring> inputText(env,
            wtfStringToJstring(env, getInputText(node), true));
    env->SetObjectField(initData, classDef->m_text, inputText.get());
    env->SetIntField(initData, classDef->m_type, getInputType(node));
    env->SetBooleanField(initData, classDef->m_isSpellCheckEnabled,
            isSpellCheckEnabled(node));
    Document* document = node->document();
//SAMSUNG CHANGE Form Navigation >>
    env->SetBooleanField(initData, classDef->m_isTextFieldNext,
            isFormNavTextInput(nextTextOrSelectNode(node)) ? true : false);
    env->SetBooleanField(initData, classDef->m_isTextFieldPrev,
            isFormNavTextInput(previousTextOrSelectNode(node)) ? true : false);
    env->SetBooleanField(initData, classDef->m_isSelectFieldNext,
            isSelectInput(nextTextOrSelectNode(node)) ? true : false);
    env->SetBooleanField(initData, classDef->m_isSelectFieldPrev,
            isSelectInput(previousTextOrSelectNode(node)) ? true : false);
//SAMSUNG CHANGE Form Navigation <<
    env->SetBooleanField(initData, classDef->m_isAutoCompleteEnabled,
            isAutoCompleteEnabled(node));
    ScopedLocalRef<jstring> fieldName(env,
            wtfStringToJstring(env, getFieldName(node), false));
    env->SetObjectField(initData, classDef->m_name, fieldName.get());
    ScopedLocalRef<jstring> label(env,
            wtfStringToJstring(env, requestLabel(document->frame(), node), false));
    env->SetObjectField(initData, classDef->m_label, label.get());
    env->SetIntField(initData, classDef->m_maxLength, getMaxLength(node));
    LayerAndroid* layer = 0;
    int layerId = platformLayerIdFromNode(node, &layer);
    IntRect bounds = absoluteClientRect(node, layer);//4.2 Merge
    ScopedLocalRef<jobject> jbounds(env, intRectToRect(env, bounds));
    env->SetObjectField(initData, classDef->m_contentBounds, jbounds.get());
    env->SetIntField(initData, classDef->m_nodeLayerId, layerId);
    IntRect contentRect;
    RenderTextControl* rtc = toRenderTextControl(node);
    if (rtc) {
        contentRect.setWidth(rtc->scrollWidth());
        contentRect.setHeight(rtc->scrollHeight());
        contentRect.move(-rtc->scrollLeft(), -rtc->scrollTop());
    }
    ScopedLocalRef<jobject> jcontentRect(env, intRectToRect(env, contentRect));
    env->SetObjectField(initData, classDef->m_clientRect, jcontentRect.get());//4.2 Merge
    return initData;
}

void WebViewCore::initEditField(Node* node)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    m_textGeneration = 0;
    int start = 0;
    int end = 0;
    getSelectionOffsets(node, start, end);
    SelectText* selectText = createSelectText(focusedFrame()->selection()->selection());
    ScopedLocalRef<jobject> initData(env, createTextFieldInitData(node));
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_initEditField,
            start, end, reinterpret_cast<int>(selectText), initData.get());
    checkException(env);
    // SAMSUNG_CHANGE [MPSG100006223] ++
    bool isPasswordField = false;
    if (node->isElementNode()) 
    {
        WebCore::Element* element = static_cast<WebCore::Element*>(node);
        if (WebCore::InputElement* inputElement = element->toInputElement())
        {
            if(node->isWMLElement())
                isPasswordField = static_cast<WebCore::WMLInputElement*>(inputElement)->isPasswordField();
            else
                isPasswordField = static_cast<WebCore::HTMLInputElement*>(inputElement)->isPasswordField();
        }
    }
    
    HTMLInputElement* input = static_cast<HTMLInputElement*>(node);
    Page* page = m_mainFrame->document()->page();
    if (!isPasswordField && input->autoComplete() 
#if ENABLE(WEB_AUTOFILL) 
		&& page->settings()->autoFillEnabled() 
#endif
	) 
    {
        setWebTextViewAutoFillableDefault();
    }
    // SAMSUNG_CHANGE [MPSG100006223] --
}

void WebViewCore::popupReply(int index)
{
    if (m_popupReply) {
        m_popupReply->replyInt(index);
//SAMSUNG CHANGE Form Navigation >>
        if(index == -2) {
           Release(m_popupReply);
           m_popupReply = 0;
        }
//SAMSUNG CHANGE Form Navigation <<
    }
}

void WebViewCore::popupReply(const int* array, int count)
{
    if (m_popupReply) {
        m_popupReply->replyIntArray(array, count);
        Release(m_popupReply);
        m_popupReply = 0;
    }
}

// This is a slightly modified Node::nextNodeConsideringAtomicNodes() with the
// extra constraint of limiting the search to inside a containing parent
WebCore::Node* nextNodeWithinParent(WebCore::Node* parent, WebCore::Node* start)
{
    if (!isAtomicNode(start) && start->firstChild())
        return start->firstChild();
    if (start->nextSibling())
        return start->nextSibling();
    const Node *n = start;
    while (n && !n->nextSibling()) {
        n = n->parentNode();
        if (n == parent)
            return 0;
    }
    if (n)
        return n->nextSibling();
    return 0;
}

void WebViewCore::initializeTextInput(WebCore::Node* node, bool fake)
{
    if (node) {
        if (isTextInput(node)) {
            bool showKeyboard = true;
            initEditField(node);
            WebCore::RenderTextControl* rtc = toRenderTextControl(node);
            if (rtc && node->hasTagName(HTMLNames::inputTag)) {
                HTMLInputElement* inputElement = static_cast<HTMLInputElement*>(node);
                bool ime = !shouldSuppressKeyboard(node) && !inputElement->readOnly();
                if (ime) {
#if ENABLE(WEB_AUTOFILL)
                    if (rtc->isTextField()) {
                        Page* page = node->document()->page();
                        EditorClient* editorClient = page->editorClient();
                        EditorClientAndroid* androidEditor =
                                static_cast<EditorClientAndroid*>(editorClient);
                        WebAutofill* autoFill = androidEditor->getAutofill();
                        autoFill->formFieldFocused(inputElement);
                    }
#endif
                } else
                    showKeyboard = false;
            }
            if (!fake)
                requestKeyboard(showKeyboard);
        } else if (!fake && !nodeIsPlugin(node)) {
            // not a text entry field, put away the keyboard.
            clearTextEntry();
        }
    } else if (!fake) {
        // There is no focusNode, so the keyboard is not needed.
        clearTextEntry();
    }
}

void WebViewCore::focusNodeChanged(WebCore::Node* newFocus)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    if (isTextInput(newFocus))
    {
//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES >>
        if(!(isDateTime(newFocus)))
            initializeTextInput(newFocus, true);
//SAMSUNG HTML5 INPUT TYPE DATE/TIME CHANGES <<
    }
    HitTestResult focusHitResult;
    focusHitResult.setInnerNode(newFocus);
    focusHitResult.setInnerNonSharedNode(newFocus);
    if (newFocus && newFocus->isLink() && newFocus->isElementNode()) {
        focusHitResult.setURLElement(static_cast<Element*>(newFocus));
        if (newFocus->hasChildNodes() && !newFocus->hasTagName(HTMLNames::imgTag)) {
            // Check to see if any of the children are images, and if so
            // set them as the innerNode and innerNonSharedNode
            // This will stop when it hits the first image. I'm not sure what
            // should be done in the case of multiple images inside one anchor...
            Node* nextNode = newFocus->firstChild();
            bool found = false;
            while (nextNode) {
                if (nextNode->hasTagName(HTMLNames::imgTag)) {
                    found = true;
                    break;
                }
                nextNode = nextNodeWithinParent(newFocus, nextNode);
            }
            if (found) {
                focusHitResult.setInnerNode(nextNode);
                focusHitResult.setInnerNonSharedNode(nextNode);
            }
        }
    }
    AndroidHitTestResult androidHitTest(this, focusHitResult);
    jobject jHitTestObj = androidHitTest.createJavaObject(env);
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_focusNodeChanged,
            reinterpret_cast<int>(newFocus), jHitTestObj);
    env->DeleteLocalRef(jHitTestObj);
}

//SAMSUNG CHANGE HTML5 COLOR <<
//Set the color value back to engine
void WebViewCore::ColorChooserReply(int color)
{
    if (m_colorChooser && color!=0) {
        m_colorChooser->didChooseColor(color); 
	IntRect r = m_colorChooser->getRect();
        contentInvalidate(r); 
	delete(m_colorChooser); 	
        m_colorChooser = 0;	
    }
    else{
        if(m_colorChooser){
	    delete(m_colorChooser); 	
	    m_colorChooser = 0;    
	}
    }	  	
}
//SAMSUNG CHANGE HTML5 COLOR <<

void WebViewCore::addMessageToConsole(const WTF::String& message, unsigned int lineNumber, const WTF::String& sourceID, int msgLevel) {
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    jstring jMessageStr = wtfStringToJstring(env, message);
    jstring jSourceIDStr = wtfStringToJstring(env, sourceID);
    env->CallVoidMethod(javaObject.get(),
            m_javaGlue->m_addMessageToConsole, jMessageStr, lineNumber,
            jSourceIDStr, msgLevel);
    env->DeleteLocalRef(jMessageStr);
    env->DeleteLocalRef(jSourceIDStr);
    checkException(env);
}

void WebViewCore::jsAlert(const WTF::String& url, const WTF::String& text)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    jstring jInputStr = wtfStringToJstring(env, text);
    jstring jUrlStr = wtfStringToJstring(env, url);
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_jsAlert, jUrlStr, jInputStr);
    env->DeleteLocalRef(jInputStr);
    env->DeleteLocalRef(jUrlStr);
    checkException(env);
}

bool WebViewCore::exceededDatabaseQuota(const WTF::String& url, const WTF::String& databaseIdentifier, const unsigned long long currentQuota, unsigned long long estimatedSize)
{
#if ENABLE(DATABASE)
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return false;
    jstring jDatabaseIdentifierStr = wtfStringToJstring(env, databaseIdentifier);
    jstring jUrlStr = wtfStringToJstring(env, url);
    env->CallVoidMethod(javaObject.get(),
            m_javaGlue->m_exceededDatabaseQuota, jUrlStr,
            jDatabaseIdentifierStr, currentQuota, estimatedSize);
    env->DeleteLocalRef(jDatabaseIdentifierStr);
    env->DeleteLocalRef(jUrlStr);
    checkException(env);
    return true;
#endif
}

// 4.2 Merge BEGIN <<
/*
 * TODO Note that this logic still needs to be cleaned up. Normally the
 * information provided in this callback is used to resize the appcache.
 * so we need to provide the current database size. However, webkit provides no
 * way to reach this information. It can be calculated by fetching all the
 * origins and their usage, however this is too expensize (requires one inner
 * join operation for each origin). Rather, we simply return the maximum cache size,
 * which should be equivalent to the current cache size. This is generally safe.
 * However, setting the maximum database size to less than the current database size
 * may cause a problem. In this case, ApplicationCacheStorage ("the owner of database")
 * uses the updated value, while database internally ignores it and uses the current size
 * as quota. This means the value we returned here won't reflect the actual database size.
 */
// 4.2 Merge END >>
bool WebViewCore::reachedMaxAppCacheSize(const unsigned long long spaceNeeded)
{
#if ENABLE(OFFLINE_WEB_APPLICATIONS)
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return false;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_reachedMaxAppCacheSize, spaceNeeded, WebCore::cacheStorage().maximumSize());//4.2 Merge
    checkException(env);
    return true;
#endif
}

void WebViewCore::populateVisitedLinks(WebCore::PageGroup* group)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    m_groupForVisitedLinks = group;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_populateVisitedLinks);
    checkException(env);
}

void WebViewCore::geolocationPermissionsShowPrompt(const WTF::String& origin)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    jstring originString = wtfStringToJstring(env, origin);
    env->CallVoidMethod(javaObject.get(),
                        m_javaGlue->m_geolocationPermissionsShowPrompt,
                        originString);
    env->DeleteLocalRef(originString);
    checkException(env);
}

// Samsung Change - HTML5 Web Notification	>>
void WebViewCore::notificationPermissionsShowPrompt(const WTF::String& url)
{
    #if ENABLE(NOTIFICATIONS)
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    ALOGV("WebViewCore::notificationPermissionsShowPrompt URL is %s",url.utf8().data());
    jstring jUrlStr = wtfStringToJstring(env, url);    
    env->CallVoidMethod(javaObject.get(),
                        m_javaGlue->m_notificationPermissionsShowPrompt,
                        jUrlStr);
    env->DeleteLocalRef(jUrlStr);
    checkException(env);
    #endif
}

void WebViewCore::notificationManagershow(const WTF::String& iconUrl, const WTF::String& titleStr, const WTF::String& bodyStr
	,int counter)
{
    #if ENABLE(NOTIFICATIONS)
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env); 
    ALOGV("Inside WebViewCore::notificationManagershow ");
    if (!javaObject.get())
        return;
    jstring jIconUrlStr = wtfStringToJstring(env, iconUrl);
    jstring jtitleStr = wtfStringToJstring(env, titleStr);
    jstring jbodyStr = wtfStringToJstring(env, bodyStr);
    env->CallVoidMethod(javaObject.get(),m_javaGlue->m_notificationManagershow,jIconUrlStr,jtitleStr,jbodyStr,
		counter);
    env->DeleteLocalRef(jIconUrlStr);
    env->DeleteLocalRef(jtitleStr);
    env->DeleteLocalRef(jbodyStr);
    checkException(env);
    #endif
}

void WebViewCore::notificationManagerCancel(int notificationID)
{
   #if ENABLE(NOTIFICATIONS)
   JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env); 
    ALOGV("Inside WebViewCore::notificationManagerCancel %d", notificationID);
    if (!javaObject.get())
        return;
   env->CallVoidMethod(javaObject.get(),m_javaGlue->m_notificationManagerCancel, notificationID);
   checkException(env);
   #endif
}

void WebViewCore::notificationPermissionsHidePrompt()
{
   #if ENABLE(NOTIFICATIONS)
   JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env); 
    ALOGV("Inside WebViewCore::notificationPermissionsHidePrompt");
    if (!javaObject.get())
        return;
   env->CallVoidMethod(javaObject.get(),m_javaGlue->m_notificationPermissionsHidePrompt);
   checkException(env);
   #endif
}
// Samsung Change - HTML5 Web Notification	<<
void WebViewCore::geolocationPermissionsHidePrompt()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_geolocationPermissionsHidePrompt);
    checkException(env);
}

jobject WebViewCore::getDeviceMotionService()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return 0;
    jobject object = env->CallObjectMethod(javaObject.get(), m_javaGlue->m_getDeviceMotionService);
    checkException(env);
    return object;
}

jobject WebViewCore::getDeviceOrientationService()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return 0;
    jobject object = env->CallObjectMethod(javaObject.get(), m_javaGlue->m_getDeviceOrientationService);
    checkException(env);
    return object;
}

bool WebViewCore::jsConfirm(const WTF::String& url, const WTF::String& text)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return false;
    jstring jInputStr = wtfStringToJstring(env, text);
    jstring jUrlStr = wtfStringToJstring(env, url);
    jboolean result = env->CallBooleanMethod(javaObject.get(), m_javaGlue->m_jsConfirm, jUrlStr, jInputStr);
    env->DeleteLocalRef(jInputStr);
    env->DeleteLocalRef(jUrlStr);
    checkException(env);
    return result;
}

bool WebViewCore::jsPrompt(const WTF::String& url, const WTF::String& text, const WTF::String& defaultValue, WTF::String& result)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return false;
    jstring jUrlStr = wtfStringToJstring(env, url);
    jstring jInputStr = wtfStringToJstring(env, text);
    jstring jDefaultStr = wtfStringToJstring(env, defaultValue);
    jstring returnVal = static_cast<jstring>(env->CallObjectMethod(javaObject.get(), m_javaGlue->m_jsPrompt, jUrlStr, jInputStr, jDefaultStr));
    env->DeleteLocalRef(jUrlStr);
    env->DeleteLocalRef(jInputStr);
    env->DeleteLocalRef(jDefaultStr);
    checkException(env);

    // If returnVal is null, it means that the user cancelled the dialog.
    if (!returnVal)
        return false;

    result = jstringToWtfString(env, returnVal);
    env->DeleteLocalRef(returnVal);
    return true;
}

//	SAMSUNG CHANGE >> Print functionality support for JS content	
void WebViewCore::printPage()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_printPage);
    checkException(env);
}
//	SAMSUNG CHANGE <<
bool WebViewCore::jsUnload(const WTF::String& url, const WTF::String& message)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return false;
    jstring jInputStr = wtfStringToJstring(env, message);
    jstring jUrlStr = wtfStringToJstring(env, url);
    jboolean result = env->CallBooleanMethod(javaObject.get(), m_javaGlue->m_jsUnload, jUrlStr, jInputStr);
    env->DeleteLocalRef(jInputStr);
    env->DeleteLocalRef(jUrlStr);
    checkException(env);
    return result;
}

bool WebViewCore::jsInterrupt()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return false;
    jboolean result = env->CallBooleanMethod(javaObject.get(), m_javaGlue->m_jsInterrupt);
    checkException(env);
    return result;
}

AutoJObject
WebViewCore::getJavaObject()
{
    return m_javaGlue->object(JSC::Bindings::getJNIEnv());
}

jobject
WebViewCore::getWebViewJavaObject()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return 0;
    return env->CallObjectMethod(javaObject.get(), m_javaGlue->m_getWebView);
}

RenderTextControl* WebViewCore::toRenderTextControl(Node* node)
{
    RenderTextControl* rtc = 0;
    RenderObject* renderer = node->renderer();
    if (renderer && renderer->isTextControl()) {
        rtc = WebCore::toRenderTextControl(renderer);
    }
    return rtc;
}

void WebViewCore::getSelectionOffsets(Node* node, int& start, int& end)
{
    RenderTextControl* rtc = toRenderTextControl(node);
    if (rtc) {
        start = rtc->selectionStart();
        end = rtc->selectionEnd();
    } else {
        // It must be content editable field.
        Document* document = node->document();
        Frame* frame = document->frame();
        SelectionController* selector = frame->selection();
        Position selectionStart = selector->start();
        Position selectionEnd = selector->end();
        Position startOfNode = firstPositionInNode(node);
        RefPtr<Range> startRange = Range::create(document, startOfNode,
                selectionStart);
        start = TextIterator::rangeLength(startRange.get(), true);
        RefPtr<Range> endRange = Range::create(document, startOfNode,
                selectionEnd);
        end = TextIterator::rangeLength(endRange.get(), true);
    }
}

String WebViewCore::getInputText(Node* node)
{
    String text;
    WebCore::RenderTextControl* renderText = toRenderTextControl(node);
    if (renderText)
        text = renderText->text();
    else {
        // It must be content editable field.
        Position start = firstPositionInNode(node);
        Position end = lastPositionInNode(node);
        VisibleSelection allEditableText(start, end);
        if (allEditableText.isRange())
            text = allEditableText.firstRange()->text();
    }
    return text;
}

//SAMSUNG CHANGES MPSG100006129 >>
void WebViewCore::updateTextSelectionStartAndEnd()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    VisibleSelection selection = focusedFrame()->selection()->selection();
    int start = 0;
    int end = 0;
    if (selection.isCaretOrRange())
        getSelectionOffsets(selection.start().anchorNode(), start, end);
    Node *focusNode = currentFocus();
    jstring text = NULL;
    if (focusNode && isTextInput(focusNode)) {
        getSelectionOffsets(focusNode, start, end);
        text = wtfStringToJstring(env, getInputText(focusNode));
    }
// Samsung Changes for Prevent defect issue ++
    SelectText* selectText = NULL;
    //SelectText* selectText = createSelectText(selection); 
    selectText = createSelectText(selection);
// Samsung Changes for Prevent defect issue --

    env->CallVoidMethod(javaObject.get(),
            m_javaGlue->m_updateTextSelectionStartAndEnd, reinterpret_cast<int>(currentFocus()),
            start, end, m_textGeneration, reinterpret_cast<int>(selectText), text);
    env->DeleteLocalRef(text);
// Samsung Changes for Prevent defect issue ++
    if(selectText){
        delete(selectText); 
        selectText = NULL;
	}
// Samsung Changes for Prevent defect issue --

    checkException(env);
}
//SAMSUNG CHANGES MPSG100006129 <<

void WebViewCore::updateTextSelection()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    VisibleSelection selection = focusedFrame()->selection()->selection();
    int start = 0;
    int end = 0;
    if (selection.isCaretOrRange())
        getSelectionOffsets(selection.start().anchorNode(), start, end);
    SelectText* selectText = createSelectText(selection);
    env->CallVoidMethod(javaObject.get(),
            m_javaGlue->m_updateTextSelection, reinterpret_cast<int>(currentFocus()),
            start, end, m_textGeneration, reinterpret_cast<int>(selectText));
    checkException(env);
}

//SAMSUNG - Google Text Selection >>
void WebViewCore::invokeUpdateTextSelection()
{
	updateTextSelection();
	return;
}
//SAMSUNG - Google Text Selection <<
void WebViewCore::updateTextSizeAndScroll(WebCore::Node* node)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    RenderTextControl* rtc = toRenderTextControl(node);
    if (!rtc)
        return;
    int width = rtc->scrollWidth();
    int height = rtc->contentHeight();
    int scrollX = rtc->scrollLeft();
    int scrollY = rtc->scrollTop();
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_updateTextSizeAndScroll,
            reinterpret_cast<int>(node), width, height, scrollX, scrollY);
    checkException(env);
}

void WebViewCore::updateTextfield(WebCore::Node* ptr, const WTF::String& text)//4.2 Merge
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    if (m_blockTextfieldUpdates)
        return;
// 4.2 Merge BEGIN <<
//Removed in 4.2
//    if (changeToPassword) {
//        env->CallVoidMethod(javaObject.get(), m_javaGlue->m_updateTextfield,
//                (int) ptr, true, 0, m_textGeneration);
//        checkException(env);
//        return;
//    }
// 4.2 Merge END >>
    jstring string = wtfStringToJstring(env, text);
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_updateTextfield,
            (int) ptr, string, m_textGeneration);//4.2 Merge
    env->DeleteLocalRef(string);
    checkException(env);
}

void WebViewCore::clearTextEntry()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_clearTextEntry);
}

void WebViewCore::setBackgroundColor(SkColor c)
{
    WebCore::FrameView* view = m_mainFrame->view();
    if (!view)
        return;

    // need (int) cast to find the right constructor
    WebCore::Color bcolor((int)SkColorGetR(c), (int)SkColorGetG(c),
                          (int)SkColorGetB(c), (int)SkColorGetA(c));

    if (view->baseBackgroundColor() == bcolor)
        return;

    view->setBaseBackgroundColor(bcolor);

    // Background color of 0 indicates we want a transparent background
    if (c == 0)
        view->setTransparent(true);

    //invalidate so the new color is shown
    contentInvalidateAll();
}

jclass WebViewCore::getPluginClass(const WTF::String& libName, const char* className)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return 0;

    jstring libString = wtfStringToJstring(env, libName);
    jstring classString = env->NewStringUTF(className);
    jobject pluginClass = env->CallObjectMethod(javaObject.get(),
                                           m_javaGlue->m_getPluginClass,
                                           libString, classString);
    checkException(env);

    // cleanup unneeded local JNI references
    env->DeleteLocalRef(libString);
    env->DeleteLocalRef(classString);

    if (pluginClass != 0) {
        return static_cast<jclass>(pluginClass);
    } else {
        return 0;
    }
}

void WebViewCore::showFullScreenPlugin(jobject childView, int32_t orientation, NPP npp)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;

    env->CallVoidMethod(javaObject.get(),
                        m_javaGlue->m_showFullScreenPlugin,
                        childView, orientation, reinterpret_cast<int>(npp));
    checkException(env);
}

void WebViewCore::hideFullScreenPlugin()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_hideFullScreenPlugin);
    checkException(env);
}

jobject WebViewCore::createSurface(jobject view)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return 0;
    jobject result = env->CallObjectMethod(javaObject.get(), m_javaGlue->m_createSurface, view);
    checkException(env);
    return result;
}

jobject WebViewCore::addSurface(jobject view, int x, int y, int width, int height)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return 0;
    jobject result = env->CallObjectMethod(javaObject.get(),
                                           m_javaGlue->m_addSurface,
                                           view, x, y, width, height);
    checkException(env);
    return result;
}

void WebViewCore::updateSurface(jobject childView, int x, int y, int width, int height)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(),
                        m_javaGlue->m_updateSurface, childView,
                        x, y, width, height);
    checkException(env);
}

void WebViewCore::destroySurface(jobject childView)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_destroySurface, childView);
    checkException(env);
}

jobject WebViewCore::getContext()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return 0;

    jobject result = env->CallObjectMethod(javaObject.get(), m_javaGlue->m_getContext);
    checkException(env);
    return result;
}

void WebViewCore::keepScreenOn(bool screenOn) {
    if ((screenOn && m_screenOnCounter == 0) || (!screenOn && m_screenOnCounter == 1)) {
        JNIEnv* env = JSC::Bindings::getJNIEnv();
        AutoJObject javaObject = m_javaGlue->object(env);
        if (!javaObject.get())
            return;
        env->CallVoidMethod(javaObject.get(), m_javaGlue->m_keepScreenOn, screenOn);
        checkException(env);
    }

    // update the counter
    if (screenOn)
        m_screenOnCounter++;
    else if (m_screenOnCounter > 0)
        m_screenOnCounter--;
}

void WebViewCore::showRect(int left, int top, int width, int height,
        int contentWidth, int contentHeight, float xPercentInDoc,
        float xPercentInView, float yPercentInDoc, float yPercentInView)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_showRect,
            left, top, width, height, contentWidth, contentHeight,
            xPercentInDoc, xPercentInView, yPercentInDoc, yPercentInView);
    checkException(env);
}

void WebViewCore::centerFitRect(int x, int y, int width, int height)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_centerFitRect, x, y, width, height);
    checkException(env);
}

void WebViewCore::setScrollbarModes(ScrollbarMode horizontalMode, ScrollbarMode verticalMode)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_setScrollbarModes, horizontalMode, verticalMode);
    checkException(env);
}

// 4.2 Merge BEGIN <<
//Removed in 4.2
//void WebViewCore::notifyWebAppCanBeInstalled()
//{
//    JNIEnv* env = JSC::Bindings::getJNIEnv();
//    AutoJObject javaObject = m_javaGlue->object(env);
//    if (!javaObject.get())
//        return;
//    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_setInstallableWebApp);
//    checkException(env);
//}
// 4.2 Merge END >>
#if ENABLE(VIDEO)
// 4.2 Merge BEGIN <
void WebViewCore::enterFullscreenForVideoLayer()
{
    // Just need to update the video mode, to avoid multiple exit full screen.
    m_fullscreenVideoMode = true;
}
// 4.2 Merge END >>

void WebViewCore::exitFullscreenVideo()
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    if (m_fullscreenVideoMode) {
        env->CallVoidMethod(javaObject.get(), m_javaGlue->m_exitFullscreenVideo);
        m_fullscreenVideoMode = false;
    }
    checkException(env);
}
#endif

void WebViewCore::setWebTextViewAutoFillable(int queryId, const string16& previewSummary)
{
#if ENABLE(WEB_AUTOFILL)
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    jstring preview = env->NewString(previewSummary.data(), previewSummary.length());
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_setWebTextViewAutoFillable, queryId, preview);
    env->DeleteLocalRef(preview);
#endif
}

// SAMSUNG_CHANGE [MPSG100006223] ++
void WebViewCore::setWebTextViewAutoFillableDefault()
{
#if ENABLE(WEB_AUTOFILL)
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    jstring preview = NULL; 
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_setWebTextViewAutoFillable, -1, preview);
    env->DeleteLocalRef(preview);
#endif
}
// SAMSUNG_CHANGE [MPSG100006223] --

bool WebViewCore::drawIsPaused() const
{
    // returning true says scrollview should be offscreen, which pauses
    // gifs. because this is not again queried when we stop scrolling, we don't
    // use the stopping currently.
    return false;
}

void WebViewCore::setWebRequestContextUserAgent()
{
    // We cannot create a WebRequestContext, because we might not know it this is a private tab or not yet
    if (m_webRequestContext)
        m_webRequestContext->setUserAgent(WebFrame::getWebFrame(m_mainFrame)->userAgentForURL(0)); // URL not used
}

void WebViewCore::setWebRequestContextCacheMode(int cacheMode)
{
    m_cacheMode = cacheMode;
    // We cannot create a WebRequestContext, because we might not know it this is a private tab or not yet
    if (!m_webRequestContext)
        return;

    m_webRequestContext->setCacheMode(cacheMode);
}

WebRequestContext* WebViewCore::webRequestContext()
{
    if (!m_webRequestContext) {
        Settings* settings = NULL;
	if(mainFrame())  // P120926-7247 Once Crash shows Null pointer access crash.
          settings = mainFrame()->settings();
        m_webRequestContext = new WebRequestContext(settings && settings->privateBrowsingEnabled());
        setWebRequestContextUserAgent();
        setWebRequestContextCacheMode(m_cacheMode);
    }
    return m_webRequestContext.get();
}

//SAMSUNG ADVANCED TEXT SELECTION - BEGIN
void WebViewCore::webTextSelectionAll(int x1, int y1, int parma1, int param2)
{
//	DEBUG_NAV_UI_LOGD("webTextSelectionAll : called  %d, %d",  x1, y1);
	//WebCore::Frame* frame = m_mainFrame;
	WebCore::Frame* frame = focusedFrame();
	if (frame->selection()) {
		//param1 Indicate  Falg for first selection need to do before select all.
		//param2 is not used .
		if(parma1 == 1) {
			// Select text First
			selectClosestWord(x1,y1,1.0f,true);
	//		DEBUG_NAV_UI_LOGD("%s: first Word Select  ", __FUNCTION__);
		}
		frame->selection()->selectAll();
		//frame->selection()->setGranularity(WebCore::ParagraphGranularity);
	} else {
//		DEBUG_NAV_UI_LOGD("%s: Exception:  Frame Selection is null ", __FUNCTION__);
	}
}

int mSelectionDirection = DirectionForward;

void WebViewCore::copyMoveSelection(int x, int y,  int controller, bool smartGranularity, bool selectionMove, float zoomLevel , int granularity)
{
//	DEBUG_NAV_UI_LOGD("%s: x,y position: %d, %d", __FUNCTION__, x, y);
	WebCore::Frame* main_frame = m_mainFrame;
	WebCore::Frame* frame = focusedFrame();

	WebCore::IntPoint contentsPoint = WebCore::IntPoint(x, y);
	WebCore::IntPoint wndPoint = main_frame->view()->contentsToWindow(contentsPoint);
//	DEBUG_NAV_UI_LOGD("%s: second time click", __FUNCTION__);
	//Set Direction
	VisibleSelection visSel = frame->selection()->selection();
//SISO_HTMLComposer start
        if(frame->document() && frame->document()->settings()
            && frame->document()->settings()->editableSupportEnabled()) {
            InlineBox *box;
            int caretOffset = 0;
            visSel.end().getInlineBoxAndOffset(visSel.affinity(), box, caretOffset);
            int hght = 0;
            if(box && box->root())
	        hght = (box->root())->logicalHeight();
            wndPoint = WebCore::IntPoint(wndPoint.x(), wndPoint.y()-(hght/2));
        }
//SISO_HTMLComposer end
	if (selectionMove == false) {
		copySetSelectionDirection(controller);
		if(frame->selection()->granularity() == WebCore::WordGranularity )
		{
			frame->selection()->setGranularity(WebCore::CharacterGranularity);
			visSel.expandUsingGranularity(WebCore::CharacterGranularity);
//			DEBUG_NAV_UI_LOGD("Changed from Word to character Granularity");
		}else   if(frame->selection()->granularity() == WebCore::CharacterGranularity )
		{
			visSel.expandUsingGranularity(WebCore::CharacterGranularity);
//			LOGD("Set the character Granularity");
		}
		return;
	}

	if (smartGranularity == true) {
		if (!inSameParagraph(WebCore::VisiblePosition(frame->selection()->base()),
				WebCore::VisiblePosition(frame->selection()->extent()))) {
			if (frame->selection()->granularity() != WebCore::ParagraphGranularity) {
				frame->selection()->setGranularity(WebCore::ParagraphGranularity);
				visSel.expandUsingGranularity(WebCore::ParagraphGranularity);
//				DEBUG_NAV_UI_LOGD("Set Paragraph Granularity");
			}
		} else {
			if (frame->selection()->granularity() == WebCore::ParagraphGranularity) {
				frame->selection()->setGranularity(WebCore::CharacterGranularity);
				visSel.expandUsingGranularity(WebCore::CharacterGranularity);
//				DEBUG_NAV_UI_LOGD("Change from Paragraph to Character Granularity");
			}
		}
		//Set In Paragraph mode when zoom level is less than 0.8
		if (zoomLevel < 0.8 && frame->selection()->granularity() != WebCore::ParagraphGranularity) {
			frame->selection()->setGranularity(WebCore::ParagraphGranularity);
			visSel.expandUsingGranularity(WebCore::ParagraphGranularity);
//			DEBUG_NAV_UI_LOGD("Set Paragraph Granularity for Less Zoom Level");
		}
	}

	WebCore::TextGranularity CurrGranulaity = frame->selection()->granularity() ;

	//User Granularity Apply if Set
	if(granularity != -1 && CurrGranulaity == WebCore::CharacterGranularity){
		frame->selection()->setGranularity((WebCore::TextGranularity) granularity );
//		LOGD("Set  Granularity by client  %d",  granularity);
		webkitCopyMoveSelection(wndPoint, contentsPoint, controller);
		frame->selection()->setGranularity((WebCore::TextGranularity) CurrGranulaity );
	}  else{
		webkitCopyMoveSelection(wndPoint, contentsPoint, controller);
	}
    // One more check to make sure that granularity matches with the current points
    if (smartGranularity == true) {
	    if(!inSameParagraph(WebCore::VisiblePosition(frame->selection()->base()),
	        WebCore::VisiblePosition(frame->selection()->extent())))
	    {
	        if (frame->selection()->granularity() != WebCore::ParagraphGranularity) {
	            frame->selection()->setGranularity(WebCore::ParagraphGranularity);
	            visSel.expandUsingGranularity(WebCore::ParagraphGranularity);
//	            DEBUG_NAV_UI_LOGD("Correct granularity to Paragraph Granularity");
	        }
	    }
	    else
	    {
	        if(frame->selection()->granularity() == WebCore::ParagraphGranularity) {
	            frame->selection()->setGranularity(WebCore::CharacterGranularity);
	            visSel.expandUsingGranularity(WebCore::CharacterGranularity);
//	            DEBUG_NAV_UI_LOGD("Correct granularity to Character Granularity");
	        }
	    }
    }
    // End

//	DEBUG_NAV_UI_LOGD("%s: End", __FUNCTION__);
}

void WebViewCore::clearTextSelection(int contentX, int contentY)
{
//            DEBUG_NAV_UI_LOGD("%s: x,y position: %d, %d", __FUNCTION__, contentX, contentY);
                WebCore::Frame* frame = focusedFrame();
                       if (frame->selection()){
                       		frame->selection()->clear();
                       } else {
//            DEBUG_NAV_UI_LOGD("%s: Exception:  Frame Selection is null ", __FUNCTION__);
                                                   }
}

void WebViewCore::clearTextSelectionIframe()
{
                //            DEBUG_NAV_UI_LOGD("%s: x,y position: %d, %d", __FUNCTION__, contentX, contentY);
                    WebCore::Frame* frame = focusedFrame();
                    while (Frame* parent = frame->tree()->parent())
                    frame = parent;
                    for (Frame* child = frame; child; child = child->tree()->traverseNext()) 
                         {
                    if (child->selection() && child != focusedFrame()){
                    	XLOGC("Rachit::clearing this child %u",child);
                    	child->selection()->clear();
                     } else {
		//            DEBUG_NAV_UI_LOGD("%s: Exception:  Frame Selection is null ", __FUNCTION__);
                	    }
                            }
}

void WebViewCore::copySetSelectionDirection(int controller)
{
//	DEBUG_NAV_UI_LOGD("%s: Set the Selection Direction: %d", __FUNCTION__, controller);

	WebCore::Frame* main_frame = m_mainFrame;
	WebCore::Frame* frame = focusedFrame();

	frame->eventHandler()->setMousePressed(true);
	frame->selection()->setIsDirectional(false); // Need to set to make selection work in all directions
	switch(controller)
	{
	case 2:
	case 5:
		mSelectionDirection = DirectionForward;
		frame->selection()->willBeModified(SelectionController::AlterationExtend, DirectionForward);
		break;

	case 3:
		mSelectionDirection = DirectionLeft;
		frame->selection()->willBeModified(SelectionController::AlterationExtend, DirectionLeft);
		break;
	case 4:
		mSelectionDirection = DirectionRight;
		frame->selection()->willBeModified(SelectionController::AlterationExtend, DirectionRight);
		break;

	case 1:
	case 6:
		mSelectionDirection = DirectionBackward;
		frame->selection()->willBeModified(SelectionController::AlterationExtend, DirectionBackward);
		break;
	default:
//		DEBUG_NAV_UI_LOGD("%s: Invalid Direction: %d", __FUNCTION__, controller);
		frame->eventHandler()->setMousePressed(false);
		break;
	}
}

void WebViewCore::webkitCopyMoveSelection(WebCore::IntPoint wndPoint, WebCore::IntPoint contentPoint, int controller)
{
//	DEBUG_NAV_UI_LOGD("%s", __FUNCTION__);
	WebCore::Frame* frame = m_mainFrame;
//	DEBUG_NAV_UI_LOGD("%s: Frame=%s", __FUNCTION__, frame);
	WebCore::FrameView *frameview = frame->view();

	if(frame->selection()->granularity() == WebCore::ParagraphGranularity)
	{
//		DEBUG_NAV_UI_LOGD("%s: Moving in Paragraph Granularity", __FUNCTION__);
		WebCore::IntRect box = WebCore::IntRect(0,0,0,0);
		int left = 0, top = 0, right = 0, bottom = 0;

		if (RefPtr<Range> range = frame->selection()->toNormalizedRange())
		{
			ExceptionCode ec = 0;
			RefPtr<Range> tempRange = range->cloneRange(ec);
			box = tempRange->boundingBox();
			left = box.x();
			top = box.y();
			right = left + box.width();
			bottom = top + box.height();
//			DEBUG_NAV_UI_LOGD("%s: BoundingRect:[%d, %d, %d, %d]", __FUNCTION__, box.x(), box.y(), box.width(), box.height());
		}
		else
		{
//			DEBUG_NAV_UI_LOGD("%s: Exception in getting Selection Region", __FUNCTION__);
			return;
		}
		switch(mSelectionDirection)
		{
		case WebCore::DirectionForward:
			contentPoint=contentPoint.shrunkTo(WebCore::IntPoint(right, contentPoint.y()));
			contentPoint=contentPoint.expandedTo(WebCore::IntPoint(left, top));
			break;

		case DirectionBackward:
			contentPoint=contentPoint.expandedTo(WebCore::IntPoint(left, contentPoint.y()));
			contentPoint=contentPoint.shrunkTo(WebCore::IntPoint(right, bottom));
			break;

		case DirectionLeft:
			contentPoint=contentPoint.expandedTo(WebCore::IntPoint(contentPoint.x(), top));
			contentPoint=contentPoint.shrunkTo(WebCore::IntPoint(right, bottom));
			break;

		case DirectionRight:
			contentPoint=contentPoint.shrunkTo(WebCore::IntPoint(contentPoint.x(), bottom));
			contentPoint=contentPoint.expandedTo(WebCore::IntPoint(left, top));
			break;

		default:
			break;
		}
	}
	else
	{
//		DEBUG_NAV_UI_LOGD("%s: Character Granularity", __FUNCTION__);
	}

//	DEBUG_NAV_UI_LOGD("%s: Point after expansion: %d, %d", __FUNCTION__, contentPoint.x(), contentPoint.y());
//	DEBUG_NAV_UI_LOGD("%s: WindowPoint: %d, %d", __FUNCTION__, wndPoint.x(), wndPoint.y());
	WebCore::PlatformMouseEvent pme(wndPoint, contentPoint, LeftButton,
			WebCore::MouseEventMoved, 0, false, true, false, false, 0);

	frameview->frame()->eventHandler()->mouseMoved(pme);
	frameview->frame()->eventHandler()->stopAutoscrollTimer();

//	DEBUG_NAV_UI_LOGD("%s: End", __FUNCTION__);
	return;
}

//arvind.maan RTL selection fix
bool WebViewCore::recordSelectionCopiedData(SkRegion* prev_region,SkRegion* region, SkIRect* startRect,
		SkIRect* endRect, int granularity ){

//	DBG_SET_LOG("start");
	WebCore::Frame* frame =  focusedFrame();
	WebCore::Frame* main_frame =  m_mainFrame;
	WebCore::IntRect box, start, end;
    WTF::Vector<IntRect> boxVector;
	int boxX, boxY, boxWidth, boxHeight, endX, endY, temp;
	bool result = false;

	RefPtr<Range> range;
	//m_contentMutex.lock();
	if ( /*SISO_HTMLCOMPOSER*/ frame->selection()->isRange() && /*SISO_HTMLCOMPOSER*/ (range = frame->selection()->toNormalizedRange()) )
	{
		ExceptionCode ec = 0;
		RefPtr<Range> tempRange = range->cloneRange(ec);

		// consider image selection also while getting the bounds.
        boxVector = tempRange->boundingBoxEx();
        if (!boxVector.isEmpty())
        {
		region->setRect(boxVector[0]);
        for (size_t i = 1; i < boxVector.size(); i++) {
            region->op(boxVector[i], SkRegion::kUnion_Op);
        }
	box=boxVector[0];
	for (size_t i = 1; i < boxVector.size(); ++i){
        	box.unite(boxVector[i]);
	}
//	DEBUG_NAV_UI_LOGD("%s: BoundingRect:[%d, %d, %d, %d]", __FUNCTION__, box.x(), box.y(), box.width(), box.height());
           prev_region->setRect(box.x(), box.y(), box.x() + box.width(), box.y() + box.height());

			start = frame->editor()->firstRectForRange(tempRange.get());
			IntPoint frameOffset(-m_scrollOffsetX, -m_scrollOffsetY);
   			frameOffset = frame->view()->windowToContents(frameOffset);
//			DEBUG_NAV_UI_LOGD("%s: StartRect:[%d, %d, %d, %d]", __FUNCTION__, start.x(), start.y(), start.width(), start.height());
			startRect->set((start.x() - frameOffset.x()), (start.y() - frameOffset.y()), (start.x() - frameOffset.x()) + start.width(),  (start.y() - frameOffset.y()) + start.height());

			end = frame->editor()->lastRectForRange(tempRange.get());
//			DEBUG_NAV_UI_LOGD("%s: EndRect:[%d, %d, %d, %d]", __FUNCTION__, end.x(), end.y(), end.width(), end.height());
			endRect->set((end.x() - frameOffset.x()), (end.y() - frameOffset.y()), (end.x() - frameOffset.x()) + end.width(),  (end.y() - frameOffset.y()) + end.height());

			// Validation of BOUND RECT X and Y
			// Validate START and END RECTs assuming that BOUND RECT is correct
			boxX = box.x();
			boxY = box.y();
			boxWidth = box.width();
			boxHeight = box.height();
			if (boxX < 0)
			{
				boxX = 0;
			}
			if (boxY < 0)
			{
				boxY = 0;
			}
			if (box.x() < 0 || box.y() < 0)
			{
				region->setRect(boxX, boxY, boxX + boxWidth, boxY + boxHeight);
			}
// SAMSUNG CHANGE >>> MPSG 5974
/*
			// Fix for Email selection handle error >>
			// Auto fit long text without wrap cases validate lastrect range selection.
			if (frame->selection()->granularity() == WebCore::CharacterGranularity ||
					frame->selection()->granularity() == WebCore::WordGranularity) {
				int boxRight = boxX + boxWidth;
				if ((box.y() == end.y()) && ((end.x() + end.width()) < boxRight)) {
					endRect->set(boxRight - 1, end.y(), boxRight,  end.y() + end.height());
//					DEBUG_NAV_UI_LOGD("%s:Validated EndRect:[%d, %d, %d, %d]", __FUNCTION__, end.x(), end.y(), end.width(), end.height());
				}
			}
*/
// SAMSUNG CHANGE <<< MPSG 5974
			// Fix for Email selection handle error <<
			// Remove the validation : have side effect in selection bound rect
			/*
         // If START RECT is not within BOUND REC,T push the START RECT to LEFT TOP corner of BOUND RECT
         // Also START RECT width and height should not be more than BOUND RECT width and height
         if (!(region->contains(*startRect)))
             {
                 temp = start.height();
                  if (temp > boxHeight)
              {
                temp = boxHeight;
              }
          endX = start.width();
             if (endX > boxWidth)
             {
                endX = boxWidth;
             }
              startRect->set(boxX, boxY, boxX + boxWidth, boxY + temp);
             }

         // If END RECT is not within BOUND RECT, push the END RECT to RIGHT BOTTOM corner of BOUND RECT
         // Also END RECT width and height should not be more than BOUND RECT width and height
         if (!(region->contains(*endRect)))
             {
                   endX = boxX + boxWidth;
           endY = boxY + boxHeight;
           temp = end.height();
           if (temp > boxHeight)
           {
            temp = boxHeight;
           }
           if (end.width() < boxWidth)
           {
            endX = endX - end.width();
           }
           endY = endY - temp;
                  endRect->set(endX, endY, boxX + boxWidth, boxY + boxHeight);
             }
			 */
			//Validation : Text selection is not happend,though engine have selection region bound.
			WTF::String str = getSelectedText();
			if(NULL == str || str.isEmpty() /*|| str == "\n"*/){
//				DEBUG_NAV_UI_LOGD("%s: text Selection is not happend", __FUNCTION__);
			}else{
				result = true;
			}
		}
		else
		{
//			DEBUG_NAV_UI_LOGD("%s: Selection Bound Rect is Empty", __FUNCTION__);
			startRect->set(0, 0, 0, 0);
			endRect->set(0, 0, 0, 0);
			frame->selection()->clear();
		}
	}
	else{
//		DEBUG_NAV_UI_LOGD("%s: recordSelectionCopiedData  is false", __FUNCTION__);
	}

	granularity = frame->selection()->granularity();
//	DEBUG_NAV_UI_LOGD("%s: Granularity: %d", __FUNCTION__, granularity);

	//m_contentMutex.unlock();

//	DBG_SET_LOG("end");
   boxVector.clear();//arvind.maan RTL selection fix
	return result;

}

int WebViewCore::getSelectionGranularity()
{
	WebCore::Frame* frame =  focusedFrame();
	return frame->selection()->granularity();
}

bool WebViewCore::getSelectionMultiColInfo()
{
	WebCore::Frame* frame =  m_mainFrame;
	bool isMultiColumn = false;
	RefPtr<Range> range;

	if (  frame->selection()->isRange() && (range = frame->selection()->toNormalizedRange()) )
	{
		ExceptionCode ec = 0;
		RefPtr<Range> tempRange = range->cloneRange(ec);

		isMultiColumn = frame->editor()->getMultiColinfoOfSelection(tempRange.get());

//		DEBUG_NAV_UI_LOGD("%s: MultiColumn info: %d", __FUNCTION__, isMultiColumn);
	}

	return isMultiColumn;
}

bool  WebViewCore::getClosestWord(IntPoint m_globalpos, IntPoint& m_mousePos)
{
    int slop =16;
     //Frame* frame = m_mainFrame ;
	Frame* frame = focusedFrame();
	IntPoint frame_point = convertGlobalContentToFrameContent(IntPoint(m_globalpos.x(), m_globalpos.y()));
    HitTestResult hitTestResult = frame->eventHandler()->hitTestResultAtPoint(frame_point,
            false, false, DontHitTestScrollbars, HitTestRequest::ReadOnly | HitTestRequest::Active, IntSize(slop, slop));

    bool found = false;
    TouchNodeData final ;

    IntRect testRect(frame_point.x() - slop, frame_point.y() - slop, 2 * slop + 1, 2 * slop + 1);

     const ListHashSet<RefPtr<Node> >& list = hitTestResult.rectBasedTestResult();

        if (list.isEmpty()) 
	{
//            DBG_NAV_LOG("Should not happen: no rect-based-test nodes found");
            return false;
        }

        frame = hitTestResult.innerNode()->document()->frame();
        Vector<TouchNodeData> nodeDataList;
        ListHashSet<RefPtr<Node> >::const_iterator last = list.end();

        for (ListHashSet<RefPtr<Node> >::const_iterator it = list.begin(); it != last; ++it) {
		Node* it_Node = it->get();

		while (it_Node) {
			if (it_Node->nodeType() == Node::TEXT_NODE) {
				found = true;
				break;
			} else {
				it_Node = it_Node->parentNode();
			}
		}

		if (found) {
			TouchNodeData newNode;
			newNode.mInnerNode = it_Node;
			IntRect rect = getAbsoluteBoundingBox(it_Node);
			newNode.mBounds = rect;
			nodeDataList.append(newNode);  
		}
		else
			continue;
        }

     	//get best intersecting rect
       final.mInnerNode = 0;
//       DBG_NAV_LOGD("Test Rect (%d %d %d %d)", testRect.x(), testRect.y(), testRect.width(), testRect.height()) ;

       int area = 0;
       Vector<TouchNodeData>::const_iterator nlast = nodeDataList.end();
       for (Vector<TouchNodeData>::const_iterator n = nodeDataList.begin(); n != nlast; ++n) 
       {
           IntRect rect = n->mBounds;
           rect.intersect(testRect);
           int a = rect.width() * rect.height();
           if (a > area)
           {
               final = *n;
               area = a;
            }
       }

     //Adjust mouse position
      IntPoint frameAdjust = IntPoint(0,0);
      if (frame != m_mainFrame) {
          frameAdjust = frame->view()->contentsToWindow(IntPoint());
          frameAdjust.move(m_scrollOffsetX, m_scrollOffsetY);
      }

      IntRect rect = final.mBounds;
       rect.move(frameAdjust.x(), frameAdjust.y());
     // adjust m_mousePos if it is not inside the returned highlight rectangle
      testRect.move(frameAdjust.x(), frameAdjust.y());
     IntPoint RectSample = IntPoint(testRect.x(), testRect.y());
     testRect.intersect(rect);

// bounding rect of node is area which cover the surrounding area of the text.
	if ((testRect.width()!=0) && (testRect.height()!=0))
	{
		m_mousePos = WebCore::IntPoint(testRect.x(), testRect.y()) ;
		return true;
	} else {
		return false;
	}
}

bool WebViewCore::selectClosestWord(int x , int y , float zoomLevel, bool flagGranularity) {
	// Check edit filed selection
	if (tryEditFieldSelection(x, y)) return false;

	WebCore::Frame* main_frame = m_mainFrame;
	WebCore::Frame* frame = focusedFrame();
	int no_of_frames = 0;
	for (Frame* child = main_frame; child; child = child->tree()->traverseNext())
		no_of_frames = no_of_frames + 1;

	if(no_of_frames > 1)
		clearTextSelectionIframe();

	WebCore::IntPoint contentsPoint = WebCore::IntPoint(x, y);
	WebCore::IntPoint wndPoint = main_frame->view()->contentsToWindow(contentsPoint);
	
	if (!frame->eventHandler()) {
		return false;
	}

	if (true /*Disable paragraph mode*/ || zoomLevel >= 0.8)	{
		WebCore::MouseEventType met1 = WebCore::MouseEventMoved;
		WebCore::PlatformMouseEvent pme1(wndPoint, contentsPoint, NoButton, met1,
				false, false, false, false, false, 0);
		bool bReturn;
		bReturn = frame->eventHandler()->sendContextMenuEventForWordSelection(pme1, flagGranularity);

		 SelectionController* selectionContrler = frame->selection();
		if (bReturn && !(selectionContrler->selection().isRange())) {
			IntPoint mousePos;
			if (getClosestWord(contentsPoint, mousePos)) {
				WebCore::IntPoint wndPoint = main_frame->view()->contentsToWindow(mousePos);
				WebCore::PlatformMouseEvent pme2(wndPoint, mousePos, NoButton, met1,
					false, false, false, false, false, 0);
				bReturn = frame->eventHandler()->sendContextMenuEventForWordSelection(pme2, flagGranularity);
			}
		}
		return bReturn;
	} else {
		WebCore::MouseEventType met = WebCore::MouseEventPressed;
		WebCore::PlatformMouseEvent pme(wndPoint, contentsPoint, LeftButton, met, 3, false, false, false, false, 0);
		return frame->eventHandler()->handleMousePressEvent(pme);
	}
}

bool WebViewCore::tryEditFieldSelection(int x , int y ) {
    HitTestResult hoverResult;
    moveMouse(x, y, &hoverResult);
    if (hoverResult.innerNode()) {
        Node* node = hoverResult.innerNode();
        Frame* frame = node->document()->frame();
        Page* page = m_mainFrame->document()->page();
        page->focusController()->setFocusedFrame(frame);
    }
    
    IntPoint point = convertGlobalContentToFrameContent(IntPoint(x, y));

    // Hit test of this kind required for this to work inside input fields
    HitTestRequest request(HitTestRequest::Active);
    HitTestResult result(point);

    focusedFrame()->document()->renderView()->layer()->hitTest(request, result);

    // Matching the logic in MouseEventWithHitTestResults::targetNode()
    Node* node = result.innerNode();
    if (!node)
        return false;
    Element* element = node->parentElement();
    if (!node->inDocument() && element && element->inDocument())
        node = element;

    SelectionController* sc = focusedFrame()->selection();
    bool wordSelected = false;
    if (!sc->contains(point) && (node->isContentEditable() || node->isTextNode()) && !result.isLiveLink()
            && node->dispatchEvent(Event::create(eventNames().selectstartEvent, true, true))) {
//+HTML_COMPOSER Fix
        if( !node->renderer() ) return false;
//-HTML_COMPOSER Fix

        VisiblePosition pos(node->renderer()->positionForPoint(result.localPoint()));
        //wordSelected = selectWordAroundPosition(node->document()->frame(), pos);

        // 
        VisibleSelection selection(pos);
        selection.expandUsingGranularity(WordGranularity);
        SelectionController* selectionController = node->document()->frame()->selection();

        if (selectionController->shouldChangeSelection(selection)) {
            if (selection.start() == selection.end()) {
                VisibleSelection emptySelection(pos);
                selectionController->setSelection(emptySelection);
                return true;
            }
        }
    }
    return false;
}
//SAMSUNG ADVANCED TEXT SELECTION - END

void WebViewCore::scrollRenderLayer(int layer, const SkRect& rect)
{
#if USE(ACCELERATED_COMPOSITING)
    GraphicsLayerAndroid* root = graphicsRootLayer();
    if (!root)
        return;

    LayerAndroid* layerAndroid = root->platformLayer();
    if (!layerAndroid)
        return;

    LayerAndroid* target = layerAndroid->findById(layer);
    if (!target)
        return;

    RenderLayer* owner = target->owningLayer();
    if (!owner)
        return;

    if (owner->isRootLayer()) {
        FrameView* view = owner->renderer()->frame()->view();
        IntPoint pt(rect.fLeft, rect.fTop);
        view->setScrollPosition(pt);
    } else
        owner->scrollToOffset(rect.fLeft, rect.fTop);
#endif
}

//SAMSUNG CHANGE +
void WebViewCore::getWebFeedLinks ( Vector<WebFeedLink*>& out )
{
    WTF::String typeRss ( "application/rss+xml" );
    WTF::String typeRdf ( "application/rdf+xml" );
    WTF::String typeAtom ( "application/atom+xml" );
    WTF::String relAlternate ( "alternate" );

    WebCore::Frame* frame = m_mainFrame ;

    ALOGV( "WebViewCore::getWebFeedLinks()" );

    while ( frame != NULL )
    {

        Document* doc = frame->document() ;

        RefPtr<WebCore::NodeList> links = doc->getElementsByTagName ( "link" ) ;
        int length = links->length() ;

        for ( int i = 0; i < length; i ++ )
        {

            WebCore::HTMLLinkElement *linkElement = static_cast<WebCore::HTMLLinkElement *> ( links->item ( i ) );
            String type = linkElement->type() ;
            String rel = linkElement->rel() ;

            if ( ( ( type == typeRss ) || ( type == typeRdf ) || ( type == typeAtom ) ) && ( rel == relAlternate ) )
            {
                String url = linkElement->href() ;
                String title = linkElement->getAttribute ( WebCore::HTMLNames::titleAttr ) ;

               ALOGV( "WebViewCore::getWebFeedLinks() type=%s, url=%s, title = %s", type.latin1().data(), url.latin1().data(), title.latin1().data() );

                out.append ( new WebFeedLink ( url, title, type) ) ;
            }

        }

        frame = frame->tree()->traverseNext() ;
    }
}
//SAMSUNG CHANGE -



Vector<VisibleSelection> WebViewCore::getTextRanges(
        int startX, int startY, int endX, int endY)
{
    // These are the positions of the selection handles,
    // which reside below the line that they are selecting.
    // Use the vertical position higher, which will include
    // the selected text.
    startY--;
    endY--;
    VisiblePosition startSelect = visiblePositionForContentPoint(startX, startY);
    VisiblePosition endSelect =  visiblePositionForContentPoint(endX, endY);
    Position start = startSelect.deepEquivalent();
    Position end = endSelect.deepEquivalent();
    Vector<VisibleSelection> ranges;
    if (!start.isNull() && !end.isNull()) {
        if (comparePositions(start, end) > 0) {
            swap(start, end); // RTL start/end positions may be swapped
        }
        Position nextRangeStart = start;
        Position previousRangeEnd;
        do {
            VisibleSelection selection(nextRangeStart, end);
            ranges.append(selection);
            previousRangeEnd = selection.end();
            nextRangeStart = nextCandidate(previousRangeEnd);
        } while (comparePositions(previousRangeEnd, end) < 0);
    }
    return ranges;
}

void WebViewCore::deleteText(int startX, int startY, int endX, int endY)
{
    Vector<VisibleSelection> ranges =
            getTextRanges(startX, startY, endX, endY);

    EditorClientAndroid* client = static_cast<EditorClientAndroid*>(
            m_mainFrame->editor()->client());
    client->setUiGeneratedSelectionChange(true);

    SelectionController* selector = m_mainFrame->selection();
    for (size_t i = 0; i < ranges.size(); i++) {
        const VisibleSelection& selection = ranges[i];
        if (selection.isContentEditable()) {
            selector->setSelection(selection, CharacterGranularity);
            Document* document = selection.start().anchorNode()->document();
            WebCore::TypingCommand::deleteSelection(document, 0);
        }
    }
    client->setUiGeneratedSelectionChange(false);
}

void WebViewCore::insertText(const WTF::String &text)
{
    WebCore::Node* focus = currentFocus();
    if (!focus || !isTextInput(focus))
        return;

    Document* document = focus->document();

    EditorClientAndroid* client = static_cast<EditorClientAndroid*>(
            m_mainFrame->editor()->client());
    if (!client)
        return;
    client->setUiGeneratedSelectionChange(true);
    WebCore::TypingCommand::insertText(document, text,
            TypingCommand::PreventSpellChecking);
    client->setUiGeneratedSelectionChange(false);
}

void WebViewCore::resetFindOnPage()
{
    m_searchText.truncate(0);
    m_matchCount = 0;
    m_activeMatchIndex = 0;
    m_activeMatch = 0;
}
//SISO_HTMLCOMPOSER start
/*
It gets the hittestresult from the main frame , find the innernoe and the renderer.
If the rendered node is an image then it get the Renderblockbound rect which is basically the image region.
 */

WebCore::IntRect WebViewCore::GetHitImageSize( int anchorX , int anchorY)
{
    WebCore::Node* node = 0;
    WebCore::IntPoint anchorPoint;
    anchorPoint = WebCore::IntPoint(anchorX, anchorY);
    WebCore::HitTestResult hitTestResult = m_mainFrame->eventHandler()-> hitTestResultAtPoint(anchorPoint, false, true);
    node = hitTestResult.innerNode();
    WebCore::IntRect webRect ;

    webRect.setX(-1);
    webRect.setY(-1);
    webRect.setWidth(-1);
    webRect.setHeight(-1);
//SISO_HTMLComposer start
    m_SelectedImageNode = 0;
//SISO_HTMLComposer end
    WebCore::RenderObject *renderer = NULL ;
    if (!node) {
        //DBG_NAV_LOG("GetHitImageSize: HitTest Result Node is NULL!");
        return webRect;
    }
    WebCore::RenderObject* nodeRenderer = node->renderer();

    if ( nodeRenderer != NULL && nodeRenderer->isRenderImage())
    {
        webRect=    getBlockBounds(node);
        //DBG_NAV_LOGD("getRenderBlockBounds  1  : node=%p result(%d, %d, %d, %d)", node, webRect.x(), webRect.y(), webRect.width(), webRect.height());

        /*Create markup of this node - The markup is used to insert at new position in case of image move -28-9-11*/
        ALOGV ( "WebViewCore::GetHitImageSize() - setting image markup datata and size " );
        String markupString = createMarkup(node);
        setImageNodeMarkup( markupString);
        m_SelectedImageNode = node;//ananta
        //+Feature_Support_SPen
        m_SelectedImageUri = hitTestResult.absoluteImageURL();
        //-Feature_Support_SPen
    }
    else
    {
        /*DBG_NAV_LOG*/ALOGV("getRenderBlockBounds: No render block found!      ");
    }

    //test code 
    IntPoint pt = IntPoint(webRect.maxX(), webRect.maxY());
    imgVisibleSelectionToReInsert = m_mainFrame->visiblePositionForPoint(pt);
    //end 

    return webRect;

}
WebCore::IntRect WebViewCore::GetSelectedImageSize()
{
	WebCore::Node* node = getSelectedImageNode();
    WebCore::IntRect webRect ;
	webRect.setX(-1);
	webRect.setY(-1);
	webRect.setWidth(-1);
	webRect.setHeight(-1);
    if(!node)
	 return webRect;	
    WebCore::RenderObject* nodeRenderer = node->renderer();
	if(node&&nodeRenderer&&nodeRenderer->isRenderImage())
		return  getBlockBounds(node);		
	return webRect;

}

void WebViewCore:: resizeImage(int width  ,int height)
{
    WebCore::RenderObject *renderer = NULL ;
    WebCore::IntRect webRect ;
    
    if (!m_SelectedImageNode) {
        /*DBG_NAV_LOG*/ALOGV("resizeImage: HitTest Result Node is NULL!");
        return ;
    }
    WebCore::RenderObject* nodeRenderer = m_SelectedImageNode->renderer();

    if ( nodeRenderer != NULL && nodeRenderer->isRenderImage())
    {

    __android_log_print(ANDROID_LOG_DEBUG,"WebviewCore","resizeImage called - width =%d  , height = %d ", width ,height);
    float pgZoomFactor =m_mainFrame->pageZoomFactor();
    ((HTMLImageElement*)m_SelectedImageNode)->setWidth((int)width/pgZoomFactor);
    ((HTMLImageElement*)m_SelectedImageNode)->setHeight((int)height/pgZoomFactor);
    }
}
//getCurrentFontSize - api to get the font size of current selection.
int WebViewCore::getCurrentFontSize()
{
    return m_mainFrame->editor()->getCurrentFontSize(); 
}
WTF::String WebViewCore::getImageNodeMarkup()
{
    return imageNodeMarkup;
}

//getCurrentFontValue - api to get the font size of current selection in pixels
int WebViewCore::getCurrentFontValue()
{
	return m_mainFrame->editor()->getCurrentFontValue();	
}

//Added for image move feature 
void WebViewCore::saveImageSelectionController(  VisibleSelection imageSelection )
{
    ALOGV(ANDROID_LOG_DEBUG,"WebviewCore","saveImageSelectionController called ");

   
        m_VisibleImageSelection = VisibleSelection();
          
    m_VisibleImageSelection = imageSelection;
}

void WebViewCore::releaseImageSelectionController()
{
    ALOGV(ANDROID_LOG_DEBUG,"WebviewCore","initImageSelectionController called ");
    m_VisibleImageSelection = VisibleSelection(); // it assign values fro no selection 

}

VisibleSelection  WebViewCore::getImageSelectionController()
{
    ALOGV(ANDROID_LOG_DEBUG,"WebviewCore","getImageSelectionController called ");
    return m_VisibleImageSelection;
}
WebCore::Node* WebViewCore::getSelectedImageNode()
{
    ALOGV(ANDROID_LOG_DEBUG,"WebviewCore","getSelectedImageNode called ");
    return m_SelectedImageNode;
}
void WebViewCore::removeSelectedImageNode()
{
    ALOGV(ANDROID_LOG_DEBUG,"WebviewCore","removeSelectedImageNode called ");
    ExceptionCode e ;

    applyCommand(RemoveNodeCommand::create(m_SelectedImageNode));
    m_SelectedImageNode->remove(e);
    __android_log_print(ANDROID_LOG_DEBUG,"WebviewCore","removeSelectedImageNode - e = %d ", e);


}

void WebViewCore::setImageNodeMarkup(String imagenodemarkup)
{
    imageNodeMarkup = imagenodemarkup;
}


void WebViewCore::SetSelectionPreviousImage(VisibleSelection imageSelection )
{
    ALOGV(ANDROID_LOG_DEBUG,"WebviewCore","SetSelectionPreviousImage called ");

    if(imageSelection.isNone())
        ALOGV(ANDROID_LOG_DEBUG,"WebviewCore","SetSelectionPreviousImage - imageSelection is NULL     ");
    

    SelectionController* selectionContrler = m_mainFrame->selection();
    if(selectionContrler != NULL && !(selectionContrler->isNone()))
    {
        ALOGV(ANDROID_LOG_DEBUG,"WebviewCore","selectionContrler->setSelection(imageSelection) called  ");
        selectionContrler->setSelection(imageSelection);
    }   
}


void WebViewCore:: setSelectionToMoveImage(bool isNewPosition){
    
    SelectionController* frameSelectionContrler = m_mainFrame->selection();
    if(frameSelectionContrler == NULL)
        return ;
    VisibleSelection newSelection;
    if(isNewPosition)
        newSelection = VisibleSelection(imageVisiblePosition);
    else
        newSelection = VisibleSelection(imgVisibleSelectionToReInsert);

     frameSelectionContrler->setSelection(newSelection); 

}

//check  the selection- if image can be moved 
bool WebViewCore:: checkSelectionToMoveImage(){


    SelectionController* frameSelectionContrler = m_mainFrame->selection();
    if(frameSelectionContrler == NULL)
        return false;
    VisibleSelection newSelection;
    newSelection = VisibleSelection(imageVisiblePosition);

     if(frameSelectionContrler->selection().isCaret())
    {
        ALOGV(ANDROID_LOG_DEBUG,"WebviewCore","checkSelectionToMoveImage - isCaret -  return true     ");
        return true ;

    }       
    else
    {
        ALOGV(ANDROID_LOG_DEBUG,"WebviewCore","checkSelectionToMoveImage -isCaret -return false   ");
        return false;
    }
    
}

// Validating the New position while moving image to insert in new position 
bool WebViewCore:: isValidePositionToMoveImage()
{
    VisibleSelection newSelection;
    newSelection = VisibleSelection(imageVisiblePosition);
    Node* anchorNode = newSelection.base().anchorNode();
    Node* imgNode = getSelectedImageNode();
    bool isSameNode=false;
    bool isCaret=checkSelectionToMoveImage();
    if(!anchorNode || !imgNode)
    return isSameNode;
    isSameNode = anchorNode->isSameNode(imgNode);
   return (!isSameNode && isCaret);
}


//SISO_HTMLCOMPOSER end

int WebViewCore::findTextOnPage(const WTF::String &text)
{
    resetFindOnPage(); // reset even if parameters are bad

    WebCore::Frame* frame = m_mainFrame;
    if (!frame)
        return 0;

    m_searchText = text;
    FindOptions findOptions = WebCore::CaseInsensitive;

    do {
        frame->document()->markers()->removeMarkers(DocumentMarker::TextMatch);
        m_matchCount += frame->editor()->countMatchesForText(text, findOptions,
            0, true);
        frame->editor()->setMarkedTextMatchesAreHighlighted(true);
        frame = frame->tree()->traverseNextWithWrap(false);
    } while (frame);
    m_activeMatchIndex = m_matchCount - 1; // prime first findNext
    return m_matchCount;
}

int WebViewCore::findNextOnPage(bool forward)
{
    if (!m_mainFrame)
        return -1;
    if (!m_matchCount)
        return -1;

    EditorClientAndroid* client = static_cast<EditorClientAndroid*>(
        m_mainFrame->editor()->client());
    client->setUiGeneratedSelectionChange(true);

    // Clear previous active match.
    if (m_activeMatch) {
        m_mainFrame->document()->markers()->setMarkersActive(
            m_activeMatch.get(), false);
    }

    FindOptions findOptions = WebCore::CaseInsensitive
        | WebCore::StartInSelection | WebCore::WrapAround;
    if (!forward)
        findOptions |= WebCore::Backwards;

    // Start from the previous active match.
    if (m_activeMatch) {
        m_mainFrame->selection()->setSelection(m_activeMatch.get());
    }

    bool found = m_mainFrame->editor()->findString(m_searchText, findOptions);
    if (found) {
        VisibleSelection selection(m_mainFrame->selection()->selection());
        if (selection.isNone() || selection.start() == selection.end()) {
            // Temporary workaround for findString() refusing to select text
            // marked "-webkit-user-select: none".
            m_activeMatchIndex = 0;
            m_activeMatch = 0;
        } else {
            // Mark current match "active".
            if (forward) {
                ++m_activeMatchIndex;
                if (m_activeMatchIndex == m_matchCount)
                    m_activeMatchIndex = 0;
            } else {
                if (m_activeMatchIndex == 0)
                    m_activeMatchIndex = m_matchCount;
                --m_activeMatchIndex;
            }
            m_activeMatch = selection.firstRange();
            m_mainFrame->document()->markers()->setMarkersActive(
                m_activeMatch.get(), true);

//SAMSUNG CHANGES: MPSG100006003, MPSG100006340 >>
            //WAS: m_mainFrame->selection()->revealSelection(
            //         ScrollAlignment::alignCenterIfNeeded, true);

            IntRect visibleContentRect = m_mainFrame->view()->visibleContentRect();
            ALOGV("findNextOnPage() -selectionType=%d ContentSize [%d, %d] visibleContentRect [%d, %d, w=%d, h=%d] ", 
                m_mainFrame->selection()->selectionType(),
                m_mainFrame->view()->contentsWidth(), m_mainFrame->view()->contentsHeight(),
                visibleContentRect.x(), visibleContentRect.y(), visibleContentRect.width(), visibleContentRect.height());

            //Send additional scroll event to webview if View height and content height is same
            if (visibleContentRect.height() == m_mainFrame->view()->contentsHeight()) {
                IntRect rectToReveal= enclosingIntRect(m_mainFrame->selection()->bounds(false));
                IntRect rectRevealExtent = VisiblePosition(m_mainFrame->selection()->extent()).absoluteCaretBounds();
                ALOGV("findNextOnPage() - rectToReveal[%d, %d, w=%d, h=%d] rectRevealExtent[%d, %d, w=%d, h=%d] ",
                    rectToReveal.x(), rectToReveal.y(), rectToReveal.width(), rectToReveal.height(),
                    rectRevealExtent.x(), rectRevealExtent.y(), rectRevealExtent.width(), rectRevealExtent.height());
                int rectGapX = 0;
                if(rectRevealExtent.x() >= rectToReveal.maxX()) {
                    rectGapX = rectRevealExtent.x() - rectToReveal.maxX();
                }
                else {
                    rectGapX = rectToReveal.maxX() - rectRevealExtent.x();
                }
                if (rectToReveal.isEmpty() || rectGapX <= 2) {
                    m_mainFrame->selection()->revealSelection(
                        ScrollAlignment::alignCenterIfNeeded, true);  
                    if (rectToReveal.isEmpty()) {
                        rectToReveal = VisiblePosition(m_mainFrame->selection()->extent()).absoluteCaretBounds();
                    }
                }
                // remove any x-scroll required as it will be taken care by revealSelection
                if (rectToReveal.x() < visibleContentRect.x()) {
                    rectToReveal.setX(visibleContentRect.x());
                } else if (rectToReveal.maxX() > visibleContentRect.maxX() 
                                   && rectToReveal.width() < visibleContentRect.width()) {
                    rectToReveal.setX(visibleContentRect.maxX()-rectToReveal.width());
                }
                ALOGV("findNextOnPage() - modified - rectToReveal[%d, %d, w=%d, h=%d] call scrollRectOnScreen!!",
                    rectToReveal.x(), rectToReveal.y(), rectToReveal.width(), rectToReveal.height());
                //Only for scrolling in y-direction
                scrollRectOnScreen(rectToReveal); 
            }else {
                m_mainFrame->selection()->revealSelection(
                    ScrollAlignment::alignCenterIfNeeded, true);
            }

//SAMSUNG CHANGES <<
        }
    }

    // Clear selection so it doesn't display.
    m_mainFrame->selection()->clear();
    client->setUiGeneratedSelectionChange(false);
    return m_activeMatchIndex;
}

String WebViewCore::getText(int startX, int startY, int endX, int endY)
{
    String text;

    Vector<VisibleSelection> ranges =
            getTextRanges(startX, startY, endX, endY);

    for (size_t i = 0; i < ranges.size(); i++) {
        const VisibleSelection& selection = ranges[i];
        if (selection.isRange()) {
            PassRefPtr<Range> range = selection.firstRange();
            String textInRange = range->text();
            if (textInRange.length() > 0) {
                if (text.length() > 0)
                    text.append('\n');
                text.append(textInRange);
            }
        }
    }

    return text;
}

/**
 * Read the persistent locale.
 */
void WebViewCore::getLocale(String& language, String& region)
{
    char propLang[PROPERTY_VALUE_MAX], propRegn[PROPERTY_VALUE_MAX];

    property_get("persist.sys.language", propLang, "");
    property_get("persist.sys.country", propRegn, "");
    if (*propLang == 0 && *propRegn == 0) {
        /* Set to ro properties, default is en_US */
        property_get("ro.product.locale.language", propLang, "en");
        property_get("ro.product.locale.region", propRegn, "US");
    }
    language = String(propLang, 2);
    region = String(propRegn, 2);
}

// 4.2 Merge BEGIN <<
// generate bcp47 identifier for the supplied language/region
static void toLanguageTag(char* output, size_t outSize, const String& language,
    const String& region) {
    if (output == NULL || outSize <= 0)
        return;
    String locale = language;
    locale.append('_');
    locale.append(region);
    char canonicalChars[ULOC_FULLNAME_CAPACITY];
    UErrorCode uErr = U_ZERO_ERROR;
    uloc_canonicalize(locale.ascii().data(), canonicalChars,
        ULOC_FULLNAME_CAPACITY, &uErr);
    if (U_SUCCESS(uErr)) {
        char likelyChars[ULOC_FULLNAME_CAPACITY];
        uErr = U_ZERO_ERROR;
        uloc_addLikelySubtags(canonicalChars, likelyChars,
            ULOC_FULLNAME_CAPACITY, &uErr);
        if (U_SUCCESS(uErr)) {
            uErr = U_ZERO_ERROR;
            uloc_toLanguageTag(likelyChars, output, outSize, FALSE, &uErr);
            if (U_SUCCESS(uErr)) {
                return;
            } else {
                ALOGD("uloc_toLanguageTag(\"%s\") failed: %s", likelyChars,
                    u_errorName(uErr));
            }
        } else {
            ALOGD("uloc_addLikelySubtags(\"%s\") failed: %s", canonicalChars,
                u_errorName(uErr));
        }
    } else {
        ALOGD("uloc_canonicalize(\"%s\") failed: %s", locale.ascii().data(),
            u_errorName(uErr));
    }
    // unable to build a proper language identifier
    output[0] = '\0';
}
// 4.2 Merge END >>
void WebViewCore::updateLocale()
{
    static String prevLang;
    static String prevRegn;
    String language;
    String region;

    getLocale(language, region);

    if ((language != prevLang) || (region != prevRegn)) {
        prevLang = language;
        prevRegn = region;
        GlyphPageTreeNode::resetRoots();
        fontCache()->invalidate();
// 4.2 Merge BEGIN <<
        char langTag[ULOC_FULLNAME_CAPACITY];
        toLanguageTag(langTag, ULOC_FULLNAME_CAPACITY, language, region);
        FontPlatformData::setDefaultLanguage(langTag);
// 4.2 Merge END >>
    }
}

//----------------------------------------------------------------------
// Native JNI methods
//----------------------------------------------------------------------
static void RevealSelection(JNIEnv* env, jobject obj, jint nativeClass)
{
    reinterpret_cast<WebViewCore*>(nativeClass)->revealSelection();
}

static jstring RequestLabel(JNIEnv* env, jobject obj, jint nativeClass,
        int framePointer, int nodePointer)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return wtfStringToJstring(env, viewImpl->requestLabel(
            (WebCore::Frame*) framePointer, (WebCore::Node*) nodePointer));
}

static void ClearContent(JNIEnv* env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->clearContent();
}
//SAMSUNG : Reader >>
static jstring applyreadability(JNIEnv *env, jobject obj,  jint nativeClass, jstring flags)
{
   WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
   return wtfStringToJstring(env, viewImpl->applyreadability(jstringToWtfString(env, flags)));
    
}
static jstring loadinitialJs(JNIEnv *env, jobject obj, jint nativeClass, jstring flags)
{
   WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return wtfStringToJstring(env, viewImpl->loadinitialJs(jstringToWtfString(env, flags)));
}
//SAMSUNG : Reader <<

static void SetSize(JNIEnv* env, jobject obj, jint nativeClass, jint width,
        jint height, jint textWrapWidth, jfloat scale, jint screenWidth,
        jint screenHeight, jint anchorX, jint anchorY, jboolean ignoreHeight)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOGV("webviewcore::nativeSetSize(%u %u)\n viewImpl: %p", (unsigned)width, (unsigned)height, viewImpl);
    ALOG_ASSERT(viewImpl, "viewImpl not set in nativeSetSize");
    viewImpl->setSizeScreenWidthAndScale(width, height, textWrapWidth, scale,
            screenWidth, screenHeight, anchorX, anchorY, ignoreHeight);
}

static void SetScrollOffset(JNIEnv* env, jobject obj, jint nativeClass,
        jboolean sendScrollEvent, jint x, jint y)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "need viewImpl");

    viewImpl->setScrollOffset(sendScrollEvent, x, y);
}

static void SetGlobalBounds(JNIEnv* env, jobject obj, jint nativeClass,
        jint x, jint y, jint h, jint v)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "need viewImpl");

    viewImpl->setGlobalBounds(x, y, h, v);
}

static jboolean Key(JNIEnv* env, jobject obj, jint nativeClass, jint keyCode,
        jint unichar, jint repeatCount, jboolean isShift, jboolean isAlt,
        jboolean isSym, jboolean isDown)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->key(PlatformKeyboardEvent(keyCode,
        unichar, repeatCount, isDown, isShift, isAlt, isSym));
}

//SAMSUNG CHANGES MPSG100006129 >>
static void UpdateTextSelectionStartAndEnd(JNIEnv* env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->updateTextSelectionStartAndEnd();
}
//SAMSUNG CHANGES MPSG100006129 <<

static void SetInitialFocus(JNIEnv* env, jobject obj, jint nativeClass,
                            jint keyDirection)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->setInitialFocus(PlatformKeyboardEvent(keyDirection,
            0, 0, false, false, false, false));
}

//SAMSUNG CHANGES: mobile page zoom scale change issue - merge from ICS >>
static void RecalcWidthAndForceLayout(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in RecalcWidthAndForceLayout");

    WebCore::Settings* s = viewImpl->mainFrame()->page()->settings();
    if (!s) {
        ALOGV("!viewImpl->mainFrame()->page()->settings()");
        return;
    }

    //recalculate width and force layout only for mobile pages	
    if(0 == s->viewportWidth()) {
        ALOGV("Do recalcWidthAndForceLayout");
        viewImpl->recalcWidthAndForceLayout();
    }
}
//SAMSUNG CHANGES <<

static void ContentInvalidateAll(JNIEnv* env, jobject obj, jint nativeClass)
{
    reinterpret_cast<WebViewCore*>(nativeClass)->contentInvalidateAll();
}

static void DeleteSelection(JNIEnv* env, jobject obj, jint nativeClass,
        jint start, jint end, jint textGeneration)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->deleteSelection(start, end, textGeneration);
}

static void SetSelection(JNIEnv* env, jobject obj, jint nativeClass,
        jint start, jint end)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->setSelection(start, end);
}

static jstring ModifySelection(JNIEnv* env, jobject obj, jint nativeClass,
        jint direction, jint granularity)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    String selectionString = viewImpl->modifySelection(direction, granularity);
    return wtfStringToJstring(env, selectionString);
}

static void ReplaceTextfieldText(JNIEnv* env, jobject obj, jint nativeClass,
    jint oldStart, jint oldEnd, jstring replace, jint start, jint end,
    jint textGeneration)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    WTF::String webcoreString = jstringToWtfString(env, replace);
    viewImpl->replaceTextfieldText(oldStart,
            oldEnd, webcoreString, start, end, textGeneration);
}

static void PassToJs(JNIEnv* env, jobject obj, jint nativeClass,
    jint generation, jstring currentText, jint keyCode,
    jint keyValue, jboolean down, jboolean cap, jboolean fn, jboolean sym)
{
    WTF::String current = jstringToWtfString(env, currentText);
    reinterpret_cast<WebViewCore*>(nativeClass)->passToJs(generation, current,
        PlatformKeyboardEvent(keyCode, keyValue, 0, down, cap, fn, sym));
}

static void ScrollFocusedTextInput(JNIEnv* env, jobject obj, jint nativeClass,
        jfloat xPercent, jint y)//4.2 Merge
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->scrollFocusedTextInput(xPercent, y);//4.2 Merge
}

static void SetFocusControllerActive(JNIEnv* env, jobject obj, jint nativeClass,
        jboolean active)
{
    ALOGV("webviewcore::nativeSetFocusControllerActive()\n");
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in nativeSetFocusControllerActive");
    viewImpl->setFocusControllerActive(active);
}

static void SaveDocumentState(JNIEnv* env, jobject obj, jint nativeClass)
{
    ALOGV("webviewcore::nativeSaveDocumentState()\n");
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in nativeSaveDocumentState");
    viewImpl->saveDocumentState(viewImpl->focusedFrame());
}
//SAMSUNG - Google Text Selection >>
static void InvokeUpdateTextSelection(JNIEnv* env, jobject obj, jint nativeClass)
{
    ALOGV("webviewcore::nativeupdateTextSelection()\n");
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in nativeupdateTextSelection");
    viewImpl->invokeUpdateTextSelection();
}
//SAMSUNG - Google Text Selection <<

void WebViewCore::addVisitedLink(const UChar* string, int length)
{
    if (m_groupForVisitedLinks)
        m_groupForVisitedLinks->addVisitedLink(string, length);
}

static void NotifyAnimationStarted(JNIEnv* env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = (WebViewCore*) nativeClass;
    viewImpl->notifyAnimationStarted();
}

static jint RecordContent(JNIEnv* env, jobject obj, jint nativeClass, jobject pt)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    SkIPoint nativePt;
    BaseLayerAndroid* result = viewImpl->recordContent(&nativePt);
    GraphicsJNI::ipoint_to_jpoint(nativePt, env, pt);
    return reinterpret_cast<jint>(result);
}

static void SendListBoxChoice(JNIEnv* env, jobject obj, jint nativeClass,
        jint choice)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in nativeSendListBoxChoice");
    viewImpl->popupReply(choice);
}

// Set aside a predetermined amount of space in which to place the listbox
// choices, to avoid unnecessary allocations.
// The size here is arbitrary.  We want the size to be at least as great as the
// number of items in the average multiple-select listbox.
#define PREPARED_LISTBOX_STORAGE 10

static void SendListBoxChoices(JNIEnv* env, jobject obj, jint nativeClass,
        jbooleanArray jArray, jint size)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in nativeSendListBoxChoices");
    jboolean* ptrArray = env->GetBooleanArrayElements(jArray, 0);
    SkAutoSTMalloc<PREPARED_LISTBOX_STORAGE, int> storage(size);
    int* array = storage.get();
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (ptrArray[i]) {
            array[count++] = i;
        }
    }
    env->ReleaseBooleanArrayElements(jArray, ptrArray, JNI_ABORT);
    viewImpl->popupReply(array, count);
}

// TODO: Move this to WebView.cpp since it is only needed there
static jstring FindAddress(JNIEnv* env, jobject obj, jstring addr,
        jboolean caseInsensitive)
{
    if (!addr)
        return 0;
    int length = env->GetStringLength(addr);
    if (!length)
        return 0;
    const jchar* addrChars = env->GetStringChars(addr, 0);
    size_t start, end;
    AddressDetector detector;
    bool success = detector.FindContent(addrChars, addrChars + length, &start, &end);
    jstring ret = 0;
    if (success)
        ret = env->NewString(addrChars + start, end - start);
    env->ReleaseStringChars(addr, addrChars);
    return ret;
}

static jint HandleTouchEvent(JNIEnv* env, jobject obj, jint nativeClass,
        jint action, jintArray idArray, jintArray xArray, jintArray yArray,
        jint count, jint actionIndex, jint metaState)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in %s", __FUNCTION__);
    jint* ptrIdArray = env->GetIntArrayElements(idArray, 0);
    jint* ptrXArray = env->GetIntArrayElements(xArray, 0);
    jint* ptrYArray = env->GetIntArrayElements(yArray, 0);
    Vector<int> ids(count);
    Vector<IntPoint> points(count);
    for (int c = 0; c < count; c++) {
        ids[c] = ptrIdArray[c];
        points[c].setX(ptrXArray[c]);
        points[c].setY(ptrYArray[c]);
    }
    env->ReleaseIntArrayElements(idArray, ptrIdArray, JNI_ABORT);
    env->ReleaseIntArrayElements(xArray, ptrXArray, JNI_ABORT);
    env->ReleaseIntArrayElements(yArray, ptrYArray, JNI_ABORT);

    return viewImpl->handleTouchEvent(action, ids, points, actionIndex, metaState);
}

static bool MouseClick(JNIEnv* env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->performMouseClick();
}

static jstring RetrieveHref(JNIEnv* env, jobject obj, jint nativeClass,
        jint x, jint y)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in %s", __FUNCTION__);
    WTF::String result = viewImpl->retrieveHref(x, y);
    if (!result.isEmpty())
        return wtfStringToJstring(env, result);
    return 0;
}

static jstring RetrieveAnchorText(JNIEnv* env, jobject obj, jint nativeClass,
        jint x, jint y)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in %s", __FUNCTION__);
    WTF::String result = viewImpl->retrieveAnchorText(x, y);
    if (!result.isEmpty())
        return wtfStringToJstring(env, result);
    return 0;
}

static jstring RetrieveImageSource(JNIEnv* env, jobject obj, jint nativeClass,
        jint x, jint y)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    WTF::String result = viewImpl->retrieveImageSource(x, y);
    return !result.isEmpty() ? wtfStringToJstring(env, result) : 0;
}

static void MoveMouse(JNIEnv* env, jobject obj, jint nativeClass, jint x, jint y)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in %s", __FUNCTION__);
    viewImpl->moveMouse(x, y);
}

static jint GetContentMinPrefWidth(JNIEnv* env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in %s", __FUNCTION__);

    WebCore::Frame* frame = viewImpl->mainFrame();
    if (frame) {
        WebCore::Document* document = frame->document();
        if (document) {
            WebCore::RenderObject* renderer = document->renderer();
            if (renderer && renderer->isRenderView()) {
                return renderer->minPreferredLogicalWidth();
            }
        }
    }
    return 0;
}

static void SetViewportSettingsFromNative(JNIEnv* env, jobject obj,
        jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in %s", __FUNCTION__);

    WebCore::Settings* s = viewImpl->mainFrame()->page()->settings();
    if (!s)
        return;

#ifdef ANDROID_META_SUPPORT
    env->SetIntField(obj, gWebViewCoreFields.m_viewportWidth, s->viewportWidth());
    env->SetIntField(obj, gWebViewCoreFields.m_viewportHeight, s->viewportHeight());
    env->SetIntField(obj, gWebViewCoreFields.m_viewportInitialScale, s->viewportInitialScale());
    env->SetIntField(obj, gWebViewCoreFields.m_viewportMinimumScale, s->viewportMinimumScale());
    env->SetIntField(obj, gWebViewCoreFields.m_viewportMaximumScale, s->viewportMaximumScale());
    env->SetBooleanField(obj, gWebViewCoreFields.m_viewportUserScalable, s->viewportUserScalable());
    env->SetIntField(obj, gWebViewCoreFields.m_viewportDensityDpi, s->viewportTargetDensityDpi());
#endif
}

static void SetBackgroundColor(JNIEnv* env, jobject obj, jint nativeClass,
        jint color)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in %s", __FUNCTION__);

    viewImpl->setBackgroundColor((SkColor) color);
}

//SISO_HTMLCOMPOSER start

static void insertImageContent(JNIEnv *env, jobject obj, jint nativeClass, jstring command )
{
      WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);

    
    if( false==viewImpl->isValidePositionToMoveImage())
    {

       //__android_log_print(ANDROID_LOG_DEBUG,"WebviewCore","checkSelectionToMoveImage fails - Image can not be moved   ");
        return;
    }

           
    //delete the Selected image node  - using node pointer
    
    viewImpl->removeSelectedImageNode();
    


    if( false==viewImpl->checkSelectionToMoveImage())
    {
        //__android_log_print(ANDROID_LOG_DEBUG,"WebviewCore","checkSelectionToMoveImage fails - Image can not be moved viewImpl->setSelectionToMoveImage(false); ");
        viewImpl->setSelectionToMoveImage(false);
    }
    else 
    {
        //Set the selection at previously saved x,y position 
           // __android_log_print(ANDROID_LOG_DEBUG,"WebviewCore","checkSelectionToMoveImage fails - Image can be moved viewImpl->setSelectionToMoveImage(true);  ");
        viewImpl->setSelectionToMoveImage(true);
   }

    //insert image at new position
    WTF::String imageNodeMarkup = viewImpl->getImageNodeMarkup();
    WTF::String commandName = jstringToWtfString(env, command); 
    viewImpl->execCommand(commandName , imageNodeMarkup);
  
}

/*
GetImageSize() - retuns the image rect
anchorX - Touch X value
anchorY - Touch Y value

Desc : This API calls GetHitImageSize() to get the rect data (left,top,height,width)
       of the image that is touched on
*/
static jobject GetImageSize(JNIEnv *env, jobject obj ,jint nativeClass, int anchorX , int anchorY)
{

     WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
     WebCore::IntRect webRect = viewImpl->GetHitImageSize(anchorX,anchorY);

    jclass rectClass = env->FindClass("android/graphics/Rect");
    ALOG_ASSERT(rectClass, "Could not find Rect class!");

    jmethodID init = env->GetMethodID(rectClass, "<init>", "(IIII)V");
   ALOG_ASSERT(init, "Could not find constructor for Rect");

    jobject rect = env->NewObject(rectClass, init, webRect.x(),webRect.y(), webRect.maxX(), webRect.maxY());
    return rect ;
}

static jobject nativeGetSelectedImageRect(JNIEnv *env, jobject obj, jint nativeClass )
{
	WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);

	   if(!viewImpl)
	   	return 0;

	     	WebCore::IntRect webRect = viewImpl->GetSelectedImageSize();

	 if(webRect.x()==-1||webRect.width()==-1)
	 	return 0;

	jclass rectClass = env->FindClass("android/graphics/Rect");
	ALOG_ASSERT(rectClass, "Could not find Rect class!");

	jmethodID init = env->GetMethodID(rectClass, "<init>", "(IIII)V");
	ALOG_ASSERT(init, "Could not find constructor for Rect");
    jobject rect = env->NewObject(rectClass, init, webRect.x(),webRect.y(), webRect.maxX(), webRect.maxY());
    return rect ;

}
static void SimulateDelKeyForCount(JNIEnv *env, jobject obj, jint nativeClass, jint count)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->simulateDelKeyForCount(count);
}

static jstring GetTextAroundCursor(JNIEnv *env, jobject obj, jint nativeClass, jint count , jboolean isBefore)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    WTF::String result = viewImpl->getTextAroundCursor(count, isBefore);
    if (!result.isEmpty())
        return wtfStringToJstring(env, result);
    return 0;
}


static void DeleteSurroundingText(JNIEnv *env, jobject obj, jint nativeClass, jint left , jint right)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->deleteSurroundingText(left , right);
}


static jobject InsertContent(JNIEnv *env, jobject obj, jint nativeClass,jstring content,jint newcursor, jboolean composing , jobject spanObj)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    WTF::String valueString = jstringToWtfString(env, content);
    int startOffset = -1;
    int endOffset = -1;

    Vector<CompositionUnderline> spanDataVec;
    jclass vector_clazz = 0;
    jmethodID vector_sizeID = 0;
    jmethodID vector_elementAtID = 0;
    vector_clazz = env->FindClass ( "java/util/Vector" );
    vector_sizeID = env->GetMethodID(vector_clazz, "size", "()I");
    vector_elementAtID = env->GetMethodID(vector_clazz, "elementAt", "(I)Ljava/lang/Object;");

    jint vecSize = env->CallIntMethod(spanObj , vector_sizeID);
    for(int dataCnt = 0 ; dataCnt < vecSize; dataCnt++)
    {
        jobject spanData = env->CallObjectMethod(spanObj , vector_elementAtID , dataCnt);
        jclass spanInfo_clazz = 0;
        spanInfo_clazz = env->FindClass ( "android/webkit/HtmlComposerInputConnection$SpanDataInfo");
        jfieldID startOffsetField = env->GetFieldID(spanInfo_clazz , "startOffset", "I");
        jfieldID endOffsetField = env->GetFieldID(spanInfo_clazz , "endOffset", "I");
        jfieldID highLightColorField = env->GetFieldID(spanInfo_clazz , "highLightColor", "I");
        jfieldID isHighlightColorField = env->GetFieldID(spanInfo_clazz , "isHighlightColor", "Z");
        jfieldID underlineColorField = env->GetFieldID(spanInfo_clazz , "underlineColor", "I");
        jfieldID underlineThicknessField = env->GetFieldID(spanInfo_clazz , "underlineThickness", "F");


        CompositionUnderline compositionDeco;
        compositionDeco.startOffset = env->GetIntField(spanData , startOffsetField);
        compositionDeco.endOffset = env->GetIntField(spanData , endOffsetField);
        compositionDeco.isHighlightColor = env->GetBooleanField(spanData , isHighlightColorField);
        compositionDeco.thickness = env->GetFloatField(spanData, underlineThicknessField);

        int color = env->GetIntField(spanData , highLightColorField);


        jclass integer_clazz = 0;
        jmethodID integer_toHexString = 0;

        integer_clazz = env->FindClass( "java/lang/Integer" );
        integer_toHexString = env->GetStaticMethodID(integer_clazz, "toHexString", "(I)Ljava/lang/String;");

        jstring highlightColor = (jstring) env->CallStaticObjectMethod(integer_clazz , integer_toHexString , color);
        WTF::String strHighlightColor = jstringToWtfString(env, highlightColor);

        while(strHighlightColor.length() < 6)
        {
            WTF::String zero("0");
            strHighlightColor.insert(zero , 0);
        }
        WTF::String rStr = strHighlightColor.substring(0 , 2);
        WTF::String gStr = strHighlightColor.substring(2 , 2);
        WTF::String bStr = strHighlightColor.substring(4 , 2);

        ALOGV("strHighlightColor rStr : %s gStr : %s bStr : %s " , rStr.utf8().data() , gStr.utf8().data() , bStr.utf8().data());
        int rVal = rStr.toIntStrict(0 , 16);
        int gVal = gStr.toIntStrict(0 , 16);
        int bVal = bStr.toIntStrict(0 , 16);
        WebCore::Color webcoreHLColor = Color(rVal , gVal , bVal );
        ALOGV("webcoreHLColor rVal : %d gVal : %d bVal : %d " , rVal , gVal , bVal);
        ALOGV("strHighlightColor %s " , strHighlightColor.utf8().data());
        compositionDeco.highLightColor = webcoreHLColor;

        color = env->GetIntField(spanData , underlineColorField);

        jstring underlineColor = (jstring) env->CallStaticObjectMethod(integer_clazz , integer_toHexString , color);
        WTF::String strUnderlineColor = jstringToWtfString(env, underlineColor);

        while(strUnderlineColor.length() < 6)
        {
            WTF::String zero("0");
            strUnderlineColor.insert(zero , 0);
        }
        rStr = strUnderlineColor.substring(0 , 2);
        gStr = strUnderlineColor.substring(2 , 2);
        bStr = strUnderlineColor.substring(4 , 2);

        ALOGV("strUnderlineColor rStr : %s gStr : %s bStr : %s " , rStr.utf8().data() , gStr.utf8().data() , bStr.utf8().data());
        rVal = rStr.toIntStrict(0 , 16);
        gVal = gStr.toIntStrict(0 , 16);
        bVal = bStr.toIntStrict(0 , 16);
        WebCore::Color webcoreULColor = Color(rVal , gVal , bVal );
        ALOGV("webcoreULColor rVal : %d gVal : %d bVal : %d " , rVal , gVal , bVal);
        ALOGV("strUnderlineColor %s " , strUnderlineColor.utf8().data());
        compositionDeco.color = webcoreULColor;

        spanDataVec.append(compositionDeco);


    }




    viewImpl->insertContent(valueString,newcursor, composing,spanDataVec,startOffset,endOffset);
    jclass selectionOffset_clazz = 0;
    jmethodID selectionOffset_initID = 0;

    selectionOffset_clazz = env->FindClass ( "android/graphics/Point" );
    selectionOffset_initID = env->GetMethodID ( selectionOffset_clazz, "<init>", "(II)V" );
    jobject jselectionOffset_Object ;
    jselectionOffset_Object = env->NewObject ( selectionOffset_clazz, selectionOffset_initID , startOffset , endOffset);
    env->DeleteLocalRef(selectionOffset_clazz);
    return jselectionOffset_Object;
}
static void  GetSelectionOffsetImage(JNIEnv *env, jobject obj, jint nativeClass )
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->getSelectionOffsetImage();
    return ;
}

static jobject GetSelectionOffset(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    int startOffset = -1;
    int endOffset = -1;
    viewImpl->getSelectionOffset(startOffset , endOffset);

    jclass selectionOffset_clazz = 0;
    jmethodID selectionOffset_initID = 0;

    selectionOffset_clazz = env->FindClass ( "android/graphics/Point" );
    selectionOffset_initID = env->GetMethodID ( selectionOffset_clazz, "<init>", "(II)V" );
    jobject jselectionOffset_Object ;
    jselectionOffset_Object = env->NewObject ( selectionOffset_clazz, selectionOffset_initID , startOffset , endOffset);
    env->DeleteLocalRef(selectionOffset_clazz);
    return jselectionOffset_Object;
}

static jstring GetBodyText(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    WTF::String result = viewImpl->getBodyText();
    if (!result.isEmpty())
        return wtfStringToJstring(env, result);
    return 0;
}

static jstring GetBodyHTML(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    WTF::String result = viewImpl->getBodyHTML();
    if (!result.isEmpty())
        return wtfStringToJstring(env, result);
    return 0;
}

static bool GetBodyEmpty(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->getBodyEmpty();
}

static int ContentSelectionType(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->contentSelectionType();
}

static void UpdateIMSelection(JNIEnv *env, jobject obj, jint nativeClass, jint curStr , jint curEnd)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->updateIMSelection(curStr  , curEnd);
}

static bool ExecCommand(JNIEnv *env, jobject obj, jint nativeClass, jstring command , jstring value)
{
    WTF::String commandString = jstringToWtfString(env, command);
    WTF::String valueString = jstringToWtfString(env, value);
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->execCommand(commandString , valueString);

}

static bool CanUndo(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->canUndo();
}

static bool CanRedo(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->canRedo();
}

static bool IsTouchedOutside(JNIEnv *env, jobject obj, jint nativeClass, jint x, jint y )
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->isTouchedOutside(x, y);
}

static void UndoRedoStateReset(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->undoRedoStateReset();
}

static void ResizeImage(JNIEnv *env, jobject obj, jint nativeClass, jint width , jint height)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->resizeImage(width  , height);
}

static int GetCurrentFontSize(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->getCurrentFontSize();
}

static int GetCurrentFontValue(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->getCurrentFontValue();
}

static bool CopyAndSaveImage(JNIEnv *env, jobject obj, jint nativeClass, jstring url)
{
    WTF::String urlString = jstringToWtfString(env, url);
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->copyAndSaveImage(urlString);

}

static jobject GetFullMarkupData(JNIEnv* env, jobject obj, jint nativeClass)
{
    jclass markupData_clazz = 0;
    jmethodID markupData_initID = 0;
    jobject jmarkup_Object ;

    jclass subPart_clazz = 0;
    jmethodID subPart_initID = 0;

    jclass vector_clazz = 0;
    jmethodID vector_initID = 0;
    jobject vector_Object;

    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    WebHTMLMarkupData* markupData = viewImpl->getFullMarkupData();

    //ANDROID_LOG_PRINT(ANDROID_LOG_DEBUG, "HTML_EDIT", " WebViewCore::nativeGetFullMarkupData() markupData = %p", markupData);

    //Create MarkupData Object
    markupData_clazz = env->FindClass ( "android/webkit/WebHTMLMarkupData" );
    markupData_initID = env->GetMethodID ( markupData_clazz, "<init>", "()V" );
    jmarkup_Object = env->NewObject ( markupData_clazz, markupData_initID);
    ALOGV ( "WebViewCore::nativeGetFullMarkupData() MarkupData Object Created ");

    //Create Vector Class
    vector_clazz = env->FindClass ( "java/util/Vector" );
    vector_initID = env->GetMethodID ( vector_clazz, "<init>", "()V" );
    vector_Object = env->NewObject ( vector_clazz, vector_initID);
    ALOGV ( "WebViewCore::nativeGetFullMarkupData() Vector Object Created ");

    //Find Class and Method ID for SubPart
    subPart_clazz = env->FindClass ( "android/webkit/WebSubPart" );
    subPart_initID = env->GetMethodID ( subPart_clazz, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)V" ); //String,String,String,String,String,long

    if(markupData != 0){

        //Fill the Vector Object with the SupPart Data
        Vector<WebSubPart> supPartList = markupData->subPartList();
        for(int i =0;i<supPartList.size();i++){
            jobject subPart_Object;
            WebSubPart subPart = supPartList[i];

            //Create SubPart Object
            jstring j_cid = env->NewString( subPart.cid().characters(), subPart.cid().length());
            jstring j_file =  env->NewString( subPart.fileName().characters(), subPart.fileName().length());
            jstring j_mime = env->NewString( subPart.mimeType().characters(), subPart.mimeType().length());
            jstring j_path = env->NewString( subPart.path().characters(), subPart.path().length());
            jstring j_uri = env->NewString( subPart.contentUri().characters(), subPart.contentUri().length());

            subPart_Object = env->NewObject ( subPart_clazz, subPart_initID, j_cid, j_file, j_mime, j_path, j_uri, subPart.fileSize());

            //Add SubPart Object to the Vector,  Adds the specified component to the end of the vector, increasing its size by one.
            env->CallVoidMethod(vector_Object, env->GetMethodID(vector_clazz, "addElement", "(Ljava/lang/Object;)V"), subPart_Object);

            env->DeleteLocalRef(j_cid);
            env->DeleteLocalRef(j_file);
            env->DeleteLocalRef(j_mime);
            env->DeleteLocalRef(j_path);
            env->DeleteLocalRef(j_uri);
            env->DeleteLocalRef(subPart_Object);
        }

        //Set the Vector Object to the WebHTMLMarkupData
        env->CallVoidMethod(jmarkup_Object, env->GetMethodID(markupData_clazz, "setSubPartList", "(Ljava/util/Vector;)V"), vector_Object);
        env->DeleteLocalRef(vector_Object);

        //Set the HTMLFragment to the WebHTMLMarkupData
        jstring j_html = env->NewString(  markupData->htmlFragment().characters(), markupData->htmlFragment().length());
        env->CallVoidMethod(jmarkup_Object, env->GetMethodID(markupData_clazz, "setHTMLFragment", "(Ljava/lang/String;)V"), j_html);
        env->DeleteLocalRef(j_html);

        jstring j_plain= env->NewString(  markupData->plainText().characters(), markupData->plainText().length());
        env->CallVoidMethod(jmarkup_Object, env->GetMethodID(markupData_clazz, "setPlainText", "(Ljava/lang/String;)V"), j_plain);
        env->DeleteLocalRef(j_plain);

        env->DeleteLocalRef(markupData_clazz);
        env->DeleteLocalRef(vector_clazz);
        env->DeleteLocalRef(subPart_clazz);

        //Currently CharSet is ignored, will be added after confirmation
        delete markupData;
    }

    return jmarkup_Object;

}

static jobject GetCursorRect(JNIEnv *env, jobject obj , jint nativeClass, jboolean giveContentRect)
{
#ifdef ANDROID_INSTRUMENT
    TimeCounterAuto counter(TimeCounter::WebViewCoreTimeCounter);
#endif
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in nativeSetSize");

    WebCore::IntRect webRect = viewImpl->getCursorRect(giveContentRect);

    jclass rectClass = env->FindClass("android/graphics/Rect");
    ALOG_ASSERT(rectClass, "Could not find Rect class!");

    jmethodID init = env->GetMethodID(rectClass, "<init>", "(IIII)V");
    ALOG_ASSERT(init, "Could not find constructor for Rect");

    jobject rect = env->NewObject(rectClass, init, webRect.x(),
            webRect.y(), webRect.maxX(), webRect.maxY());
    return rect ;
}

static void SetSelectionNone(JNIEnv *env, jobject obj,jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->setSelectionNone();
}

static bool GetSelectionNone(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->getSelectionNone();
}

static void SetComposingSelectionNone(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->setComposingSelectionNone();
}
static void SetEditable(JNIEnv *env, jobject obj , jint nativeClass, jboolean isEditable)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->setEditable(isEditable);
}

static void SetSelectionEditable(JNIEnv *env, jobject obj, jint nativeClass, jint start , jint end)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->setSelectionEditable(start , end);
}
static void SetSelectionEditableImage(JNIEnv *env, jobject obj,jint nativeClass, jint start , jint end)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->setSelectionEditableImage(start , end);
}
static void SetComposingRegion(JNIEnv *env, jobject obj,jint nativeClass, jint start , jint end)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->setComposingRegion(start , end);
}
static void SetPageZoom(JNIEnv *env, jobject obj,jint nativeClass, jfloat factor)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->setPageZoomFact(factor);
}
static void MoveSingleCursorHandler(JNIEnv *env, jobject obj, jint nativeClass, jint x, jint y )
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->moveSingleCursorHandler(x,y );
}

static int CheckSelectionAtBoundry(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->checkSelectionAtBoundry();
}

static void SaveSelectionController(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->saveSelectionController();
}

static void ResetSelectionController(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->resetSelectionController();
}

static void RestorePreviousSelectionController(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->restorePreviousSelectionController();
}

static void CheckSelectedClosestWord( JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->checkSelectedClosestWord();
}

static int GetStateInRichlyEditableText(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->getStateInRichlyEditableText();    
}

static int CheckEndOfWordAtPosition( JNIEnv *env, jobject obj, jint nativeClass, jint x, jint y )
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->checkEndofWordAtPosition(x, y);
}

//+Feature_Support_SPen
static jstring GetSelectedImageUri( JNIEnv *env, jobject obj, jint nativeClass )
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);

    WTF::String result = viewImpl->getSelectedImageUri();
    if (!result.isEmpty())
        return wtfStringToJstring(env, result);
    return 0;
}
//-Feature_Support_SPen
//+Feature_SPen_Gesture_TextSelection
static void SelectBWStartAndEnd( JNIEnv *env, jobject obj, jint nativeClass, jint startX, jint startY, jint endX, jint endY )
{
//    WebViewCore* viewImpl = GET_NATIVE_VIEW(env, obj);
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->selectBWStartAndEnd(startX, startY, endX, endY);
}
//-Feature_SPen_Gesture_TextSelection

static void SetCursorFromRangeSelectionController(JNIEnv *env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->setCursorFromRangeSelectionController();
}

static int IsAtBoundary( JNIEnv *env, jobject obj, jint nativeClass, jint x, jint y )
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->isAtBoundary(x, y);
}

//+Feature_Drag&Drop
static void DropTheDraggedText(JNIEnv *env, jobject obj, jint nativeClass, jint x, jint y )
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->dropTheDraggedText(x,y );
}
//-Feature_Drag&Drop

static jstring GetSelectedHTMLText( JNIEnv *env, jobject obj, jint nativeClass )
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);

    WTF::String result = viewImpl->getSelectedHTMLText();
    if (!result.isEmpty())
        return wtfStringToJstring(env, result);
    return 0;
}
//SISO_HTMLCOMPOSER end

static void DumpDomTree(JNIEnv* env, jobject obj, jint nativeClass,
        jboolean useFile)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in %s", __FUNCTION__);

    viewImpl->dumpDomTree(useFile);
}

static void DumpRenderTree(JNIEnv* env, jobject obj, jint nativeClass,
        jboolean useFile)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in %s", __FUNCTION__);

    viewImpl->dumpRenderTree(useFile);
}

static void SetJsFlags(JNIEnv* env, jobject obj, jint nativeClass, jstring flags)
{
    WTF::String flagsString = jstringToWtfString(env, flags);
    WTF::CString utf8String = flagsString.utf8();
    WebCore::ScriptController::setFlags(utf8String.data(), utf8String.length());
}


// Called from the Java side to set a new quota for the origin or new appcache
// max size in response to a notification that the original quota was exceeded or
// that the appcache has reached its maximum size.
static void SetNewStorageLimit(JNIEnv* env, jobject obj, jint nativeClass,
        jlong quota)
{
#if ENABLE(DATABASE) || ENABLE(OFFLINE_WEB_APPLICATIONS)
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    Frame* frame = viewImpl->mainFrame();

    // The main thread is blocked awaiting this response, so now we can wake it
    // up.
    ChromeClientAndroid* chromeC = static_cast<ChromeClientAndroid*>(frame->page()->chrome()->client());
    chromeC->wakeUpMainThreadWithNewQuota(quota);
#endif
}

// Called from Java to provide a Geolocation permission state for the specified origin.
static void GeolocationPermissionsProvide(JNIEnv* env, jobject obj,
        jint nativeClass, jstring origin, jboolean allow, jboolean remember)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->geolocationManager()->provideRealClientPermissionState(jstringToWtfString(env, origin), allow, remember);//4.2 Merge : commented for compilation errors
}

// Samsung Change - HTML5 Web Notification	>>
// Called from Java to provide a Notification permission state for the specified origin.
static void NotificationPermissionsProvide(JNIEnv* env, jobject obj, jstring origin, jboolean allow) {
#if ENABLE(NOTIFICATIONS)
    String orgiginstr = jstringToWtfString(env, origin);
    ALOGV("NotificationPermissionsProvide origin = %s, allow = %d", orgiginstr.utf8().data(),allow);
    WebViewCore* viewImpl = GET_NATIVE_VIEW(env, obj);
    Frame* frame = viewImpl->mainFrame();

    ChromeClientAndroid* chromeClient = static_cast<ChromeClientAndroid*>(frame->page()->chrome()->client());
    chromeClient->provideNotificationPermissions(jstringToWtfString(env, origin), allow);
#endif
}

// Called from Java to send Notification event back for the specified origin.
    static void NotificationResponseback(JNIEnv* env, jobject obj ,jstring eventName, jint pointer) {
#if ENABLE(NOTIFICATIONS)
    String event = jstringToWtfString(env, eventName);
    
    ALOGV("Inside NotificationResponseback COUNTER %d" ,pointer );
    WebViewCore* viewImpl = GET_NATIVE_VIEW(env, obj);
    Frame* frame = viewImpl->mainFrame();
    ChromeClientAndroid* chromeClient = static_cast<ChromeClientAndroid*>(frame->page()->chrome()->client());
    chromeClient->dispatchNotificationEvents(event,pointer);
 #endif
     
}

//Called from Java to send Noification ID back for specified origin
static void NotificationIDback(JNIEnv* env, jobject obj, jint notificationID, jint counter) {
    
 #if ENABLE(NOTIFICATIONS)  
    ALOGV("Inside NotificationIDback NOTIFICATIONID %d COUNTER %d" ,notificationID, counter );
    WebViewCore* viewImpl = GET_NATIVE_VIEW(env, obj);
    Frame* frame = viewImpl->mainFrame();
    ChromeClientAndroid* chromeClient = static_cast<ChromeClientAndroid*>(frame->page()->chrome()->client());
    chromeClient->recordNotificationID(notificationID, counter);
#endif
}
// Samsung Change - HTML5 Web Notification	<<


static void RegisterURLSchemeAsLocal(JNIEnv* env, jobject obj, jint nativeClass,
        jstring scheme)
{
    WebCore::SchemeRegistry::registerURLSchemeAsLocal(jstringToWtfString(env, scheme));
}

// 4.2 Merge BEGIN <<
//Following code removed in 4.2
//static bool FocusBoundsChanged(JNIEnv* env, jobject obj, jint nativeClass)
//{
//    return reinterpret_cast<WebViewCore*>(nativeClass)->focusBoundsChanged();
//}

//static void SetIsPaused(JNIEnv* env, jobject obj, jint nativeClass,
//        jboolean isPaused)
//{
//    // tell the webcore thread to stop thinking while we do other work
//    // (selection and scrolling). This has nothing to do with the lifecycle
//    // pause and resume.
//    reinterpret_cast<WebViewCore*>(nativeClass)->setIsPaused(isPaused);
//}
// 4.2 Merge END >>
static void Pause(JNIEnv* env, jobject obj, jint nativeClass)
{
    // This is called for the foreground tab when the browser is put to the
    // background (and also for any tab when it is put to the background of the
    // browser). The browser can only be killed by the system when it is in the
    // background, so saving the Geolocation permission state now ensures that
    // is maintained when the browser is killed.
    GeolocationPermissions::maybeStorePermanentPermissions();//4 .2 merge : commented for compilation error

    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ChromeClient* chromeClient = viewImpl->mainFrame()->page()->chrome()->client();
    ChromeClientAndroid* chromeClientAndroid = static_cast<ChromeClientAndroid*>(chromeClient);
//    chromeClientAndroid->storeGeolocationPermissions();//4.2 Merge : removed in 4.2

// Samsung Change - HTML5 FileSystem API	>>
    chromeClientAndroid->storeFileSystemQuotaUsage();
// Samsung Change - HTML5 FileSystem API	<<
// Samsung Change - HTML5 Web Notification	>>
    chromeClientAndroid->storeNotificationPermissions();
// Samsung Change - HTML5 Web Notification	<<
    Frame* mainFrame = viewImpl->mainFrame();
// 4.2 Merge BEGIN <<
//Following code removed in 4.2
//    for (Frame* frame = mainFrame; frame; frame = frame->tree()->traverseNext()) {
//        Geolocation* geolocation = frame->domWindow()->navigator()->optionalGeolocation();
//        if (geolocation)
//            geolocation->suspend();
//    }
// 4.2 Merge END >>
    if (mainFrame)
        mainFrame->settings()->setMinDOMTimerInterval(BACKGROUND_TIMER_INTERVAL);

    viewImpl->deviceMotionAndOrientationManager()->maybeSuspendClients();
    viewImpl->geolocationManager()->suspendRealClient();//4.2 Merge : commented for compilation error

    ANPEvent event;
    SkANP::InitEvent(&event, kLifecycle_ANPEventType);
    event.data.lifecycle.action = kPause_ANPLifecycleAction;
    viewImpl->sendPluginEvent(event);
////////SERI - WebGL Changes - Pause WEbGL Contents >>
    if (mainFrame && mainFrame->document())
    {
        //mainFrame->document()->documentWillBecomeInactive();
        mainFrame->document()->pauseScriptedAnimations();
    }
////////SERI - WebGL Changes - Pause WEbGL Contents << 

//    viewImpl->setIsPaused(true);//4.2 Merge : removed in 4.2
}

static void Resume(JNIEnv* env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    Frame* mainFrame = viewImpl->mainFrame();
// 4.2 Merge BEGIN <<
//    for (Frame* frame = mainFrame; frame; frame = frame->tree()->traverseNext()) {
//        Geolocation* geolocation = frame->domWindow()->navigator()->optionalGeolocation();
//        if (geolocation)
//            geolocation->resume();
//    }
// 4.2 Merge END >>
    if (mainFrame)
        mainFrame->settings()->setMinDOMTimerInterval(FOREGROUND_TIMER_INTERVAL);

    viewImpl->deviceMotionAndOrientationManager()->maybeResumeClients();
  viewImpl->geolocationManager()->resumeRealClient();//4.2 Merge: commented for compilation error

    ANPEvent event;
    SkANP::InitEvent(&event, kLifecycle_ANPEventType);
    event.data.lifecycle.action = kResume_ANPLifecycleAction;
    viewImpl->sendPluginEvent(event);
////////SERI - WebGL Changes - Pause WEbGL Contents >>
    if (mainFrame && mainFrame->document())
    {
        //mainFrame->document()->documentDidBecomeActive();
        mainFrame->document()->resumeScriptedAnimations();
    }
////////SERI - WebGL Changes - Pause WEbGL Contents <<

//    viewImpl->setIsPaused(false);//4.2 Merge : removed in 4.2
}

static void FreeMemory(JNIEnv* env, jobject obj, jint nativeClass)
{
    ANPEvent event;
    SkANP::InitEvent(&event, kLifecycle_ANPEventType);
    event.data.lifecycle.action = kFreeMemory_ANPLifecycleAction;
    reinterpret_cast<WebViewCore*>(nativeClass)->sendPluginEvent(event);
}

static void ProvideVisitedHistory(JNIEnv* env, jobject obj, jint nativeClass,
        jobject hist)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOG_ASSERT(viewImpl, "viewImpl not set in %s", __FUNCTION__);

    jobjectArray array = static_cast<jobjectArray>(hist);

    jsize len = env->GetArrayLength(array);
    for (jsize i = 0; i < len; i++) {
        jstring item = static_cast<jstring>(env->GetObjectArrayElement(array, i));
        const UChar* str = static_cast<const UChar*>(env->GetStringChars(item, 0));
        jsize len = env->GetStringLength(item);
        viewImpl->addVisitedLink(str, len);
        env->ReleaseStringChars(item, str);
        env->DeleteLocalRef(item);
    }
}

static void PluginSurfaceReady(JNIEnv* env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    if (viewImpl)
        viewImpl->sendPluginSurfaceReady();
}

// Notification from the UI thread that the plugin's full-screen surface has been discarded
static void FullScreenPluginHidden(JNIEnv* env, jobject obj, jint nativeClass,
        jint npp)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    PluginWidgetAndroid* plugin = viewImpl->getPluginWidget((NPP)npp);
    if (plugin)
        plugin->exitFullScreen(false);
}

static jobject HitTest(JNIEnv* env, jobject obj, jint nativeClass, jint x,
                       jint y, jint slop, jboolean doMoveMouse)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    if (!viewImpl)
        return 0;
    AndroidHitTestResult result = viewImpl->hitTestAtPoint(x, y, slop, doMoveMouse);
    return result.createJavaObject(env);
}

static void AutoFillForm(JNIEnv* env, jobject obj, jint nativeClass,
        jint queryId)
{
#if ENABLE(WEB_AUTOFILL)
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    if (!viewImpl)
        return;

    WebCore::Frame* frame = viewImpl->mainFrame();
    if (frame) {
        EditorClientAndroid* editorC = static_cast<EditorClientAndroid*>(frame->page()->editorClient());
        WebAutofill* autoFill = editorC->getAutofill();
        autoFill->fillFormFields(queryId);
    }
#endif
}

// SAMSUNG CHANGE Copy image begin >>
static bool SaveCachedImageToFile(JNIEnv *env, jobject obj, jstring url, jstring filePath)
{
    WTF::String strUrl = jstringToWtfString(env, url);
    WTF::String strPath = jstringToWtfString(env, filePath);

    WebViewCore* viewImpl = GET_NATIVE_VIEW(env, obj);
    return viewImpl->saveCachedImageToFile( strUrl, strPath );
}

bool WebViewCore::saveCachedImageToFile(WTF::String& imageUrl, WTF::String& filePath)
{
    return WebCore::saveCachedImageToFile( m_mainFrame, imageUrl, filePath );
}
// SAMSUNG CHANGE Copy image end <<
//SAMSUNG ADVANCED TEXT SELECTION - BEGIN
static void WebTextSelectionAll(JNIEnv *env, jobject obj, jint nativeClass,jint x1, jint y1, jint x2, jint y2)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
	ALOG_ASSERT(viewImpl, "view not set in %s", __FUNCTION__);
	viewImpl->webTextSelectionAll(x1, y1, x2, y2);
}

static void CopyMoveSelection(JNIEnv *env, jobject obj, jint nativeClass,jint x, jint y, jint controller,
		jboolean ex, jboolean selectionMove, jfloat zoomLevel, jint granularity)
{
       WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
	ALOG_ASSERT(viewImpl, "view not set in %s", __FUNCTION__);
	viewImpl->copyMoveSelection(x, y, controller, ex, selectionMove, zoomLevel, granularity);
}

static void ClearTextSelection(JNIEnv *env, jobject obj, jint nativeClass,jint contentX, jint contentY)
{
        WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
	ALOG_ASSERT(viewImpl, "view not set in %s", __FUNCTION__);
	viewImpl->clearTextSelection(contentX, contentY);
}


static bool RecordSelectionCopiedData(JNIEnv *env, jobject obj,jint nativeClass, jobject region,jobject cRegion,  jobject sRect,jobject eRect, jint value)
{
#ifdef ANDROID_INSTRUMENT
	TimeCounterAuto counter(TimeCounter::WebViewCoreTimeCounter);
#endif
        WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
	ALOG_ASSERT(viewImpl, "viewImpl not set in RecordSelectionCopiedData");

	SkRegion* nativeRegion = GraphicsJNI::getNativeRegion(env, region);
    SkRegion* cNativeRegion = GraphicsJNI::getNativeRegion(env, cRegion);//arvind.maan RTL selection fix
	SkIRect nativeSRect;
	SkIRect nativeERect;

    bool result = viewImpl->recordSelectionCopiedData(nativeRegion,cNativeRegion, &nativeSRect,&nativeERect,value);
	GraphicsJNI::set_jrect(env,sRect,nativeSRect.fLeft, nativeSRect.fTop,nativeSRect.fRight, nativeSRect.fBottom );
	GraphicsJNI::set_jrect(env,eRect,nativeERect.fLeft, nativeERect.fTop,nativeERect.fRight, nativeERect.fBottom );


	//GraphicsJNI::iRect_to_jRect(&nativeSRect, env,sRect);
	//GraphicsJNI::iRect_to_jRect(&nativeERect, env,eRect);
	return result;
}

static int GetSelectionGranularity(JNIEnv *env, jobject obj,jint nativeClass)
{
        WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
	ALOG_ASSERT(viewImpl, "viewImpl not set in GetSelectionGranularity");
	return viewImpl->getSelectionGranularity();
}

static bool GetSelectionMultiColInfo(JNIEnv *env, jobject obj,jint nativeClass)
{
        WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
	ALOG_ASSERT(viewImpl, "viewImpl not set in GetSelectionMultiColInfo");
	return viewImpl->getSelectionMultiColInfo();
}

static jstring GetSelectedText(JNIEnv *env, jobject obj,jint nativeClass)
{
#ifdef ANDROID_INSTRUMENT
	TimeCounterAuto counter(TimeCounter::WebViewCoreTimeCounter);
#endif
       WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
	ALOG_ASSERT(viewImpl, "viewImpl not set in %s", __FUNCTION__);
	WTF::String result = viewImpl->getSelectedText();
	if (!result.isEmpty())
		return wtfStringToJstring(env, result);
	return 0;
}

// SAMSUNG CHANGE ++ GET_INPUT_TEXT_BOUNDS
static void GetInputTextBounds(JNIEnv* env, jobject obj, jint nativeClass,
		jobject contentBounds)
{
	WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    IntRect bounds = viewImpl->getInputTextBounds();
    if (contentBounds)
        GraphicsJNI::irect_to_jrect(bounds, env, contentBounds);
}
// SAMSUNG CHANGE --

static bool SelectClosestWord(JNIEnv *env, jobject obj,jint nativeClass,jint x, jint y, jfloat zoomLevel, jboolean flag)
{
	WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
	ALOG_ASSERT(viewImpl, "viewImpl not set in SelectClosestWord");
	return viewImpl->selectClosestWord(x,y, zoomLevel, flag);

}
//SAMSUNG ADVANCED TEXT SELECTION - END

static void CloseIdleConnections(JNIEnv* env, jobject obj, jint nativeClass)
{
    WebCache::get(true)->closeIdleConnections();
    WebCache::get(false)->closeIdleConnections();
}

static void nativeCertTrustChanged(JNIEnv *env, jobject obj)
{
    WebCache::get(true)->certTrustChanged();
    WebCache::get(false)->certTrustChanged();
}

static void ScrollRenderLayer(JNIEnv* env, jobject obj, jint nativeClass,
        jint layer, jobject jRect)
{
    SkRect rect;
    GraphicsJNI::jrect_to_rect(env, jRect, &rect);
    reinterpret_cast<WebViewCore*>(nativeClass)->scrollRenderLayer(layer, rect);
}

//SAMSUNG CHANGES >>
static jobjectArray nativeGetWebFeedLinks ( JNIEnv* env, jobject obj, jint nativeClass )
{
    jclass fi_clazz = 0;
    jmethodID initID = 0;
    jobjectArray infos ;
    int start = 0;
    int limit = 0;
    jobject urlobj,titleobj, typeobj;//, pathobj ;
    Vector<WebFeedLink*> feedInfoList ;

    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);

    viewImpl->getWebFeedLinks ( feedInfoList ) ;

   ALOGV( "WebViewCore::nativeGetWebFeedLinks() links count = %d", feedInfoList.size() );

    fi_clazz = env->FindClass ( "android/webkit/WebFeedLink" );
    initID = env->GetMethodID ( fi_clazz, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );

    infos = env->NewObjectArray ( feedInfoList.size(), fi_clazz, NULL );

    for ( int i = 0; i <  feedInfoList.size(); i++ )
    {
        urlobj =env->NewString ( feedInfoList[i]->url().characters(), feedInfoList[i]->url().length() );
        titleobj = env->NewString ( feedInfoList[i]->title().characters(), feedInfoList[i]->title().length() );
        typeobj = env->NewString ( feedInfoList[i]->type().characters(), feedInfoList[i]->type().length() );
        //pathobj = env->NewString(feedInfoList[i]->path().characters(), feedInfoList[i]->path().length());

        jobject fi = env->NewObject ( fi_clazz, initID, urlobj, titleobj, typeobj );

        env->SetObjectArrayElement ( infos, i, fi );

        delete feedInfoList[i] ;
        env->DeleteLocalRef(urlobj);
        env->DeleteLocalRef(titleobj);
        env->DeleteLocalRef(typeobj);
        env->DeleteLocalRef(fi);

        start = limit;
    }

    feedInfoList.clear() ;

    return infos ;
}

//SAMSUNG CHANGES <<


static void DeleteText(JNIEnv* env, jobject obj, jint nativeClass,
        jint startX, jint startY, jint endX, jint endY)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->deleteText(startX, startY, endX, endY);
}

static void InsertText(JNIEnv* env, jobject obj, jint nativeClass,
        jstring text)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    WTF::String wtfText = jstringToWtfString(env, text);
    viewImpl->insertText(wtfText);
}

static jobject GetText(JNIEnv* env, jobject obj, jint nativeClass,
        jint startX, jint startY, jint endX, jint endY)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    WTF::String text = viewImpl->getText(startX, startY, endX, endY);
    return text.isEmpty() ? 0 : wtfStringToJstring(env, text);
}

static void SelectText(JNIEnv* env, jobject obj, jint nativeClass,
        jint handleId, jint x, jint y)//4.2 Merge
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->selectText(static_cast<SelectText::HandleId>(handleId), x, y);//4.2 Merge
}
//SAMSUNG CHANGES >>> SPELLCHECK(sataya.m@samsung.com)
#if ENABLE(SPELLCHECK)
long nextWordStartIndex(char* sentence)
{
	long wordstartindex = -1;
	long i;
	
	for(i = 0;sentence[i]!='\0';i++)
	{
		if((sentence[i]>='a' && sentence[i]<='z') || (sentence[i]>='A' &&sentence[i]<='Z'))
		{	
			break;
		}
	}
	
	if(sentence[i]!='\0')
		wordstartindex = i;
		
	return wordstartindex;
}
#endif
//SAMSUNG CHANGES <<<	
    
//SAMSUNG CHANGES >>> SPELLCHECK(sataya.m@samsung.com)
static bool nativeCheckSpellingOfWordAtPosition(JNIEnv *env, jobject obj, jint nativeClass,jint x, jint y) {
#if ENABLE(SPELLCHECK)
    jint location = -1, length = 0;
	long wordstartindex = 0;
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->selectClosestWord(x,y, 1.0f, true);
    WTF::String result = viewImpl->getSelectedText();
	android_printLog(ANDROID_LOG_DEBUG, "WebViewCore","WebViewCore:nativeCheckSpellingOfWordAtPosition Text to select %s %d",result.utf8().data(),result.length());
	
	wordstartindex = nextWordStartIndex(const_cast<char*>(result.ascii().data()));
	
	if((result.length() == 0 )|| (wordstartindex == -1))
		return false;

    if (result != NULL) {
        ALOGV("selected word at position: %s", result.utf8().data());
    }
    else {
        ALOGV("No word selected");
    }
  

    WebCore::Frame* frame = viewImpl->mainFrame();
    if (frame) {
        EditorClientAndroid* editorC = static_cast<EditorClientAndroid*>(frame->page()->editorClient());
        editorC->checkSpellingOfString(result.characters(), result.length(), &location, &length);//SAMSUNG CHANGES
    }
    
    if ((location == -1) && (length == 0)) {
        return false;
    } else {
        return true;
    }
#else
    ALOGV("Spell Check feature not enabled: define the macro ENABLE_SPELLCHECK");
    return false;
#endif
	return true;
}

static void nativeUnmarkWord(JNIEnv *env, jobject obj, jint nativeClass, jstring word)
{
#if ENABLE(SPELLCHECK)
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    ALOGV("nativeUnmarkWord");
    WebCore::Frame* frame = viewImpl->mainFrame();
    if (frame) {
    EditorClientAndroid* editorC = static_cast<EditorClientAndroid*>(frame->page()->editorClient());
    editorC->unmarkwordlist(jstringToWtfString(env, word));
    }
#else
    ALOGV("Spell Check feature not enabled: define the macro ENABLE_SPELLCHECK");
#endif
}
//SAMSUNG CHANGES <<<	

static void ClearSelection(JNIEnv* env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->focusedFrame()->selection()->clear();
}

//SAMSUNG : Reader >>
//Called from browser activity and webview on click of reader button. Evaluates the reader javascript
WTF ::String WebViewCore::applyreadability(WTF::String  str)
{
#if USE(V8)
//	LOGV("in apply readability");
        m_mainFrame->script()->executeScript(str);
   //     LOGV("after evaluation of script");
			
        Element* iFrame = m_mainFrame->document()->getElementById(WTF::String("reader_iframe"));
        HTMLFrameOwnerElement* reader_frame= static_cast<HTMLFrameOwnerElement *>(iFrame);
        //Element* divElement = reader_frame->contentDocument()->getElementById(WTF::String("reader_div"));
        if(reader_frame && (reader_frame->contentWindow()!=NULL)  && (reader_frame->contentWindow()->document()!=NULL))
        {
		Element* divElement = reader_frame->contentWindow()->document()->getElementById(WTF::String("reader_div")); // Changes for P120618-4508 
	        HTMLElement* HTMLdivElement= static_cast<HTMLElement *>(divElement);
        	return HTMLdivElement->innerHTML();
        }
	else
		return WTF::String();
#endif
}

//Evaluates the initial recognizearticle.js
WTF::String WebViewCore::loadinitialJs(WTF::String  str)
{
#if USE(V8)
	//LOGV("in load initial JS");
        m_mainFrame->script()->executeScript(str);
	//LOGV("after evaluation of script");
	Element* divElement = m_mainFrame-> document()->getElementById(WTF::String("recog_div"));
	HTMLElement* HTMLdivElement= static_cast<HTMLElement *>(divElement);
	return HTMLdivElement->innerHTML().utf8().data() ;
#endif
}
//SAMSUNG : Reader <<

static bool SelectWordAt(JNIEnv* env, jobject obj, jint nativeClass, jint x, jint y)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->selectWordAt(x, y);
}

static void SelectAll(JNIEnv* env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    viewImpl->focusedFrame()->selection()->selectAll();
}

static int FindAll(JNIEnv* env, jobject obj, jint nativeClass,
        jstring text)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    WTF::String wtfText = jstringToWtfString(env, text);
    return viewImpl->findTextOnPage(wtfText);
}

static int FindNext(JNIEnv* env, jobject obj, jint nativeClass,
        jboolean forward)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->findNextOnPage(forward);
}

//SAMSUNG CHANGE HTML5 COLOR <<
static void SendColorPickerChoice(JNIEnv* env, jobject obj,jint nativeClass, jint choice)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);  
    viewImpl->ColorChooserReply(choice);
}
//SAMSUNG CHANGE HTML5 COLOR >>

//SAMSUNG CHANGE Form Navigation >>
bool WebViewCore::performClickOnNode(Node* node) {
    if (!node) return false;
    LayerAndroid* layer = 0;
    platformLayerIdFromNode(node, &layer);
    // MPSG100006196 >>
    IntRect bounds = absoluteClientRect(node, layer);
    if(layer) {
       RenderLayer *owner = layer->owningLayer();
       if(owner && !owner->isRootLayer()) {
    	  bounds = getAbsoluteBoundingBox(node);
       }
    }
    // MPSG100006196 <<

    if (bounds.x() == 0 && bounds.y() == 0 && bounds.isEmpty()) return false;
    ALOGD("WebViewCore::performClickOnNode bounds(%d, %d, %d, %d), Center(%d, %d)", bounds.x(), bounds.y(), bounds.maxX(), bounds.maxY(), bounds.center().x(), bounds.center().y());
    moveMouse(bounds.center().x(), bounds.center().y(), 0, true);
    return performMouseClick();
}

bool WebViewCore::moveFocusToNext() {
    ALOGD("WebViewCore::moveFocusToNext");
    WebCore::Node* focusNode = currentFocus();
    if (!focusNode) return false;
    if(!isFormNavTextInput(focusNode) && !isSelectInput(focusNode)) {
        ALOGD("Focused Node is not Text or Select Input. Focused node name is <%s> and tag name is <%s>", 
            focusNode->nodeName().utf8().data(), 
            ((Element*) focusNode)->tagName().utf8().data());
        return false;
    }
    WebCore::Node* newNode = nextTextOrSelectNode(focusNode);
    if (!newNode) return false;

    if(!isFormNavTextInput(newNode) && !isSelectInput(newNode)) {
            ALOGD("New Node is not Text or Select Input. New node name is <%s> and tag name is <%s>",
            		newNode->nodeName().utf8().data(),
                ((Element*) newNode)->tagName().utf8().data());
            return false;
    }

    scrollNodeIntoView(m_mainFrame, newNode);
    bool handled =  performClickOnNode(newNode);
    if (isFormNavTextInput(newNode)) {
//SAMSUNG CHANGES: MPSG100006345 - FindOnPage cursor display issue >>
        //moved the focus setting logic here from performMouseClick() as it is only applicable to form navigation prev-next
	    WebCore::Frame* focusedFrame = focusNode->document()->frame();
	    if (focusedFrame) {
            focusedFrame->page()->focusController()->setFocused(true);
            focusedFrame->page()->focusController()->setActive(true);
	    }
//SAMSUNG CHANGES <<
        RenderTextControl *rtc = toRenderTextControl(newNode);
        if(NULL == rtc)
		{
			
				WTF::String str = ((Element*)newNode)->innerText();
				int len = str.length();
				setSelection(newNode, len);
		}
		else  
        setSelection(newNode, rtc->text().length(), rtc->text().length());
    }
    return handled;
}

bool WebViewCore::moveFocusToPrevious() {
    ALOGD("WebViewCore::moveFocusToPrevious");
    WebCore::Node* focusNode = currentFocus();
    if (!focusNode) return false;
    if(!isFormNavTextInput(focusNode) && !isSelectInput(focusNode)) {
        ALOGD("Focused Node is not Text or Select Input. Focused node name is <%s> and tag name is <%s>", 
            focusNode->nodeName().utf8().data(), 
            ((Element*) focusNode)->tagName().utf8().data());
        return false;
    }
    WebCore::Node* newNode = previousTextOrSelectNode(focusNode);
    if (!newNode) return false;

    if(!isFormNavTextInput(newNode) && !isSelectInput(newNode)) {
            ALOGD("New Node is not Text or Select Input. New node name is <%s> and tag name is <%s>",
            		newNode->nodeName().utf8().data(),
                ((Element*) newNode)->tagName().utf8().data());
            return false;
    }

    scrollNodeIntoView(m_mainFrame, newNode);
    bool handled = performClickOnNode(newNode);
    if (isFormNavTextInput(newNode)) {
//SAMSUNG CHANGES: MPSG100006345 - FindOnPage cursor display issue >>
        //moved the focus setting logic here from performMouseClick() as it is only applicable to form navigation prev-next
	    WebCore::Frame* focusedFrame = focusNode->document()->frame();
	    if (focusedFrame) {
            focusedFrame->page()->focusController()->setFocused(true);
            focusedFrame->page()->focusController()->setActive(true);
	    }
//SAMSUNG CHANGES <<
        RenderTextControl *rtc = toRenderTextControl(newNode);
        if(NULL == rtc)
		{
			
				WTF::String str = ((Element*)newNode)->innerText();
				int len = str.length();
				setSelection(newNode, len);
		}
		else 
        setSelection(newNode, rtc->text().length(), rtc->text().length());
    }
    return handled;
}

static bool MoveFocusToNext(JNIEnv* env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->moveFocusToNext();
}

static bool MoveFocusToPrevious(JNIEnv* env, jobject obj, jint nativeClass)
{
    WebViewCore* viewImpl = reinterpret_cast<WebViewCore*>(nativeClass);
    return viewImpl->moveFocusToPrevious();
}

jobject WebViewCore::createSelectFieldInitData(Node* node)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    TextFieldInitDataGlue* classDef = m_textFieldInitDataGlue;
    ScopedLocalRef<jclass> clazz(env,
            env->FindClass("android/webkit/WebViewCore$TextFieldInitData"));
    jobject initData = env->NewObject(clazz.get(), classDef->m_constructor);
    env->SetIntField(initData, classDef->m_fieldPointer,
            reinterpret_cast<int>(node));	
    env->SetBooleanField(initData, classDef->m_isTextFieldNext,
            isFormNavTextInput(nextTextOrSelectNode(node)) ? true : false);
    env->SetBooleanField(initData, classDef->m_isTextFieldPrev,
            isFormNavTextInput(previousTextOrSelectNode(node)) ? true : false);
    env->SetBooleanField(initData, classDef->m_isSelectFieldNext,
            isSelectInput(nextTextOrSelectNode(node)) ? true : false);
    env->SetBooleanField(initData, classDef->m_isSelectFieldPrev,
            isSelectInput(previousTextOrSelectNode(node)) ? true : false);
    LayerAndroid* layer = 0;
    platformLayerIdFromNode(node, &layer);
    IntRect bounds = absoluteClientRect(node, layer);
    ScopedLocalRef<jobject> jbounds(env, intRectToRect(env, bounds));
    env->SetObjectField(initData, classDef->m_contentBounds, jbounds.get());
    return initData;
}

void WebViewCore::initSelectField(Node* node)
{
    JNIEnv* env = JSC::Bindings::getJNIEnv();
    AutoJObject javaObject = m_javaGlue->object(env);
    if (!javaObject.get())
        return;
    ScopedLocalRef<jobject> initData(env, createSelectFieldInitData(node));
    env->CallVoidMethod(javaObject.get(), m_javaGlue->m_initSelectField, initData.get());
    checkException(env);
}

static Node *nextDocumentFocusableNode(Node *frameOwner)
{
	if (NULL == frameOwner) {
	     return NULL;
	}

	HTMLFrameOwnerElement *frameOwnerElement = static_cast<HTMLFrameOwnerElement *>(frameOwner);
	Document *ownerDocument = frameOwnerElement->contentDocument();
	if (NULL == ownerDocument) {
	    return NULL;
	}

	Node *docBody = ownerDocument->body();
	if (isContentEditable(docBody)) {
		return docBody;
	}
	
	Node *nextNode = docBody;
	while(nextNode) {
		nextNode = nextNode->traverseNextNode();
		if (nextNode && (/* nextNode->isFrameOwnerElement() || */ nextNode->hasTagName(HTMLNames::iframeTag) ||
	     	nextNode->hasTagName(HTMLNames::frameTag))) {
	     	ALOGD("We got a frameOwner Element");
		    HTMLFrameOwnerElement *nestedFrameOwner = static_cast<HTMLFrameOwnerElement *>(nextNode);
		    if (!nestedFrameOwner->contentFrame()) {
		    	ALOGD("WebViewCore::nestedFrameFocusableFormNode : frame doesn't have source and hence skipped");
            	continue;
		    }

		    nextNode = nextDocumentFocusableNode(nestedFrameOwner);
		    if (NULL == nextNode) {
		        nextNode = static_cast<Node *>(nestedFrameOwner);
		        continue;
		    }
		}
		
		if(nextNode && (nextNode->isFocusable()) && (isFormNavTextInput(nextNode) || isSelectInput(nextNode))) {
           ALOGD("Found focusable node from nested");
           break;
		}
	}

	return nextNode;
}
Node* WebViewCore::nextTextOrSelectNode(Node* node) {
    Document* document = node->document();
    if (NULL == document) {
        ALOGD("WebViewCore::nextTextOrSelectNode : !document");
        return NULL;
    }

    Node *nextNode = node;
    while (nextNode ) {
        nextNode = nextNode->traverseNextNode();
        if (nextNode && (/* nextNode->isFrameOwnerElement() || */nextNode->hasTagName(HTMLNames::iframeTag)  ||
            nextNode->hasTagName(HTMLNames::frameTag))) {
            HTMLFrameOwnerElement *frameOwner = static_cast<HTMLFrameOwnerElement *>(nextNode);
            if (!frameOwner->contentFrame()) {
                ALOGD("WebViewCore::nextTextOrSelectNode : frame doesn't have source and hence skipped");
                continue;
            }
             
            nextNode = nextDocumentFocusableNode(nextNode);
            if (NULL == nextNode) {
                nextNode = static_cast<Node *>(frameOwner);
                ALOGD("WebViewCore::nextTextOrSelectNode : Assigned frameOwner %s Back", 
                    nextNode->nodeName().utf8().data());
                continue;
            }
        }
        
        if (NULL == nextNode) {// Couldn't find anything in the current document scope, try finding in other document scope if present any
            if (document->frame() != m_mainFrame) {
                ALOGD("WebViewCore::nextTextOrSelectNode : We are checking other document scope");
                nextNode = nextTextOrSelectNode(node->document()->ownerElement());
            }
            else {
                //Only One document Scope Didn't find anything
            }
        }

        if (nextNode && (nextNode->isFocusable()) && (isFormNavTextInput(nextNode) || isSelectInput(nextNode))) {
            ALOGD("WebViewCore::nextTextOrSelectNode : Broken after finding a focusable Node");
           break;
    }
    }

    if (!nextNode) {
        ALOGD("WebViewCore::nextTextOrSelectNode :  nextNode is NULL");
        return NULL;
    }
    else {
        ALOGD("WebViewCore::nextTextOrSelectNode : Final Next Nodename = <%s>, tagname = <%s>", 
   	    nextNode->nodeName().utf8().data(), ((Element*) nextNode)->tagName().utf8().data());
    }

    return nextNode;
}

static Node *previousDocumentFocusableNode(Node *documentOwner) {
    ALOGD("Inside previousDocumentFocusableNode()");
    if (NULL == documentOwner) {
         return NULL;
    }

    HTMLFrameOwnerElement *frameOwnerElement = static_cast<HTMLFrameOwnerElement *>(documentOwner);
    Document *ownerDocument = frameOwnerElement->contentFrame()->document();
    if (NULL == ownerDocument) {
        return NULL;
    }

    Node *docLastChild = ownerDocument->lastChild();
    while (docLastChild && docLastChild->hasChildNodes()) {
           docLastChild = docLastChild->lastChild();
    }
    
    if (isContentEditable(docLastChild)) { //In case of empty body Element
        return docLastChild;
    }

    Node *previousNode = docLastChild;
    while (previousNode) {
        previousNode = previousNode->traversePreviousNode();
    	if (previousNode && (/* previousNode->isFrameOwnerElement() || */previousNode->hasTagName(HTMLNames::iframeTag) ||
            previousNode->hasTagName(HTMLNames::frameTag))) {
            ALOGD("We got a frameOwner Element");
            HTMLFrameOwnerElement *nestedFrameOwner = static_cast<HTMLFrameOwnerElement *>(previousNode);
            if (!nestedFrameOwner->contentFrame()) {
                ALOGD("WebViewCore::nestedFrameFocusableFormNode : frame doesn't have source and hence skipped");
                continue;
            }


            previousNode = previousDocumentFocusableNode(nestedFrameOwner);
		    if (NULL == previousNode) {
		        previousNode = static_cast<Node *>(nestedFrameOwner);
		        continue;
		    }
    	}

    	if (previousNode && (previousNode->isFocusable()) && (isFormNavTextInput(previousNode) || isSelectInput(previousNode))) {
            ALOGD("Found focusable node from nested");
            break;
    	}
    }

    return previousNode;
}

Node* WebViewCore::previousTextOrSelectNode(Node* node) {
    Document* document = node->document();
    if (!document) {
        ALOGD("WebViewCore::previousTextOrSelectNode : !document");
        return NULL;
    }

    Node *previousNode = node;
    while (previousNode) {
        previousNode = previousNode->traversePreviousNode();
        if (previousNode && (/* previousNode->isFrameOwnerElement() ||*/previousNode->hasTagName(HTMLNames::iframeTag)  ||
            previousNode->hasTagName(HTMLNames::frameTag))) {
            ALOGD("WebViewCore::previousTextOrSelectNode : Found an iFrame");
            HTMLFrameOwnerElement *frameOwner = static_cast<HTMLFrameOwnerElement *>(previousNode);
            if (!frameOwner->contentFrame()) {
                ALOGD("WebViewCore::previousTextOrSelectNode : frame doesn't have source and hence skipped");
            	continue;
            }
		
            previousNode = previousDocumentFocusableNode(previousNode);
            if (NULL == previousNode) {
                previousNode = static_cast<Node *>(frameOwner);
                ALOGD("WebViewCore::previousTextOrSelectNode : Assigned frameOwner %s Back", 
				previousNode->nodeName().utf8().data());
                continue;
            }
        }

        if (NULL == previousNode) {
            if (document->frame() != m_mainFrame) {
                ALOGD("Continue with previous Frame hierarchy");
                previousNode = previousTextOrSelectNode(node->document()->ownerElement());
    	    }

            else {
                //We are in Top frame nothing found, standing at first focusable Node.
            }
        }
        
        if (previousNode && (previousNode->isFocusable()) && (isFormNavTextInput(previousNode) || isSelectInput(previousNode))) {
            ALOGD("WebViewCore::previousTextOrSelectNode : Broken after finding a focusable Node");
           break;
    }
 
   }

    if (!previousNode) {
        ALOGD("WebViewCore::previousTextOrSelectNode :  !previousNode )");
        return NULL;
    }
 
    ALOGD("WebViewCore::previousTextOrSelectNode : Final Previous Nodename = <%s>, tagname = <%s>", 
    previousNode->nodeName().utf8().data(), ((Element*) previousNode)->tagName().utf8().data());
    return previousNode;
}
//SAMSUNG CHANGE Form Navigation <<

// ----------------------------------------------------------------------------

/*
 * JNI registration.
 */
static JNINativeMethod gJavaWebViewCoreMethods[] = {
    { "nativeClearContent", "(I)V",
            (void*) ClearContent },
//    { "nativeFocusBoundsChanged", "(I)Z",//4.2 Merge : removed in 4.2
//        (void*) FocusBoundsChanged } ,//4.2 Merge : removed in 4.2
    { "nativeKey", "(IIIIZZZZ)Z",
        (void*) Key },
    { "nativeContentInvalidateAll", "(I)V",
        (void*) ContentInvalidateAll },
    { "nativeSendListBoxChoices", "(I[ZI)V",
        (void*) SendListBoxChoices },
    { "nativeSendListBoxChoice", "(II)V",
        (void*) SendListBoxChoice },
    { "nativeSetSize", "(IIIIFIIIIZ)V",
        (void*) SetSize },
    { "nativeSetScrollOffset", "(IZII)V",
        (void*) SetScrollOffset },
//SAMSUNG CHANGES >>> SPELLCHECK(sataya.m@samsung.com)
#if ENABLE(SPELLCHECK)
    { "nativeUnmarkWord", "(ILjava/lang/String;)V",
        (void*) nativeUnmarkWord },
#endif
//SAMSUNG CHANGES <<<	
    { "nativeSetGlobalBounds", "(IIIII)V",
        (void*) SetGlobalBounds },
    { "nativeSetSelection", "(III)V",
        (void*) SetSelection } ,
    { "nativeModifySelection", "(III)Ljava/lang/String;",
        (void*) ModifySelection },
    { "nativeDeleteSelection", "(IIII)V",
        (void*) DeleteSelection } ,
    { "nativeReplaceTextfieldText", "(IIILjava/lang/String;III)V",
        (void*) ReplaceTextfieldText } ,
    { "nativeMoveMouse", "(III)V",
        (void*) MoveMouse },
    { "passToJs", "(IILjava/lang/String;IIZZZZ)V",
        (void*) PassToJs },
    { "nativeScrollFocusedTextInput", "(IFI)V",//4.2 Merge
        (void*) ScrollFocusedTextInput },
// SAMSUNG - Google Text selection >>
    { "nativeupdateTextSelection", "(I)V",
        (void*) InvokeUpdateTextSelection},
// SAMSUNG - Google Text selection <<
    { "nativeSetFocusControllerActive", "(IZ)V",
        (void*) SetFocusControllerActive },
    { "nativeSaveDocumentState", "(I)V",
        (void*) SaveDocumentState },
    { "nativeFindAddress", "(Ljava/lang/String;Z)Ljava/lang/String;",
        (void*) FindAddress },
    { "nativeHandleTouchEvent", "(II[I[I[IIII)I",
        (void*) HandleTouchEvent },
    { "nativeMouseClick", "(I)Z",
        (void*) MouseClick },
    { "nativeRetrieveHref", "(III)Ljava/lang/String;",
        (void*) RetrieveHref },
    { "nativeRetrieveAnchorText", "(III)Ljava/lang/String;",
        (void*) RetrieveAnchorText },
    { "nativeRetrieveImageSource", "(III)Ljava/lang/String;",
        (void*) RetrieveImageSource },
    { "nativeGetContentMinPrefWidth", "(I)I",
        (void*) GetContentMinPrefWidth },
    { "nativeNotifyAnimationStarted", "(I)V",
        (void*) NotifyAnimationStarted },
    { "nativeRecordContent", "(ILandroid/graphics/Point;)I",
        (void*) RecordContent },
    { "setViewportSettingsFromNative", "(I)V",
        (void*) SetViewportSettingsFromNative },
    { "nativeSetBackgroundColor", "(II)V",
        (void*) SetBackgroundColor },
//SISO_HTMLCOMPOSER start
    { "nativeInsertContent","(ILjava/lang/String;IZLjava/util/Vector;)Landroid/graphics/Point;",
        (void*) InsertContent},
    { "nativeSimulateDelKeyForCount", "(II)V",
     (void*) SimulateDelKeyForCount },

    { "nativeGetTextAroundCursor", "(IIZ)Ljava/lang/String;",
     (void*) GetTextAroundCursor },

    { "nativeDeleteSurroundingText", "(III)V",
        (void*) DeleteSurroundingText },


    { "nativeGetSelectionOffset", "(I)Landroid/graphics/Point;",
        (void*) GetSelectionOffset },

    { "nativeGetSelectionOffsetImage", "(I)V",
        (void*) GetSelectionOffsetImage },

    { "nativeGetBodyText", "(I)Ljava/lang/String;",
        (void*) GetBodyText },

    { "nativeExecCommand", "(ILjava/lang/String;Ljava/lang/String;)Z",
     (void*) ExecCommand },

    { "nativeCanUndo", "(I)Z",
     (void*) CanUndo },

    { "nativeCanRedo", "(I)Z",
     (void*) CanRedo },

    { "nativeisTouchedOutside", "(III)Z",
     (void*) IsTouchedOutside },

    { "nativeUndoRedoStateReset", "(I)V",
     (void*) UndoRedoStateReset },


    { "nativeCopyAndSaveImage", "(ILjava/lang/String;)Z",
     (void*) CopyAndSaveImage },




    { "nativeGetBodyHTML", "(I)Ljava/lang/String;",
        (void*) GetBodyHTML },

    { "nativeGetFullMarkupData", "(I)Landroid/webkit/WebHTMLMarkupData;",
        (void*) GetFullMarkupData },

    { "nativeSetEditable", "(IZ)V",
        (void*) SetEditable },

    { "nativeSetSelectionEditable", "(III)V",
        (void*) SetSelectionEditable },

    { "nativeMoveSingleCursorHandler", "(III)V",
        (void*) MoveSingleCursorHandler },

    { "nativeSetComposingRegion", "(III)V",
        (void*) SetComposingRegion },
    { "nativeSetPageZoom", "(IF)V",
	(void*) SetPageZoom },
    { "nativeGetCursorRect", "(IZ)Landroid/graphics/Rect;",
        (void*) GetCursorRect },

    { "nativeSetSelectionNone", "(I)V",
        (void*) SetSelectionNone },

    { "nativeGetSelectionNone", "(I)Z",
        (void*) GetSelectionNone },

    { "nativeSetComposingSelectionNone", "(I)V",
        (void*) SetComposingSelectionNone },

    { "nativeGetBodyEmpty", "(I)Z",
     (void*) GetBodyEmpty },


    { "nativeContentSelectionType", "(I)I",
     (void*) ContentSelectionType },

    { "nativeUpdateIMSelection", "(III)V",
     (void*) UpdateIMSelection },

    { "nativeRestorePreviousSelectionController", "(I)V",
     (void*) RestorePreviousSelectionController },

    { "nativeResetSelectionController", "(I)V",
     (void*) ResetSelectionController },

    { "nativeSaveSelectionController", "(I)V",
     (void*) SaveSelectionController },

    { "nativeCheckSelectionAtBoundry", "(I)I",
     (void*) CheckSelectionAtBoundry },

    { "nativeCheckSelectedClosestWord", "(I)V", 
     (void*) CheckSelectedClosestWord },

    { "nativeGetStateInRichlyEditableText", "(I)I",
     (void*) GetStateInRichlyEditableText },    

    { "nativeinsertImageContent", "(ILjava/lang/String;)V",
     (void*) insertImageContent },

    { "nativeresizeImage", "(III)V",
     (void*) ResizeImage },

    { "nativegetCurrentFontSize", "(I)I",
     (void*) GetCurrentFontSize },


    { "nativegetCurrentFontValue", "(I)I",
     (void*) GetCurrentFontValue },


    { "nativeCheckEndOfWordAtPosition", "(III)I",
     (void*) CheckEndOfWordAtPosition },
     
    { "nativeGetImageSize", "(III)Landroid/graphics/Rect;",
        (void*) GetImageSize },

    { "nativeGetSelectedImageRect", "(I)Landroid/graphics/Rect;",
     (void*) nativeGetSelectedImageRect },

//+Feature_Support_SPen
    { "nativeGetSelectedImageUri", "(I)Ljava/lang/String;",
     (void*) GetSelectedImageUri },
//-Feature_Support_SPen

//+Feature_SPen_Gesture_TextSelection
    { "nativeSelectBWStartAndEnd", "(IIIII)V",
     (void*) SelectBWStartAndEnd },
//-Feature_SPen_Gesture_TextSelections

    { "nativeSetCursorFromRangeSelectionController", "(I)V",
     (void*) SetCursorFromRangeSelectionController },

    { "nativeIsAtBoundary", "(III)I",
     (void*) IsAtBoundary },

//+Feature_Drag&Drop
    { "nativeDropTheDraggedText", "(III)V",
     (void*) DropTheDraggedText },
//-Feature_Drag&Drop

    { "nativeGetSelectedHTMLText", "(I)Ljava/lang/String;",
     (void*) GetSelectedHTMLText },
//SISO_HTMLCOMPOSER end
    { "nativeRegisterURLSchemeAsLocal", "(ILjava/lang/String;)V",
        (void*) RegisterURLSchemeAsLocal },
    { "nativeDumpDomTree", "(IZ)V",
        (void*) DumpDomTree },
    { "nativeDumpRenderTree", "(IZ)V",
        (void*) DumpRenderTree },
    { "nativeSetNewStorageLimit", "(IJ)V",
        (void*) SetNewStorageLimit },
    { "nativeGeolocationPermissionsProvide", "(ILjava/lang/String;ZZ)V",
        (void*) GeolocationPermissionsProvide },
//    { "nativeSetIsPaused", "(IZ)V", (void*) SetIsPaused },//4.2 Merge : removed in 4.2
    { "nativePause", "(I)V", (void*) Pause },
    { "nativeResume", "(I)V", (void*) Resume },
    { "nativeFreeMemory", "(I)V", (void*) FreeMemory },
    { "nativeSetJsFlags", "(ILjava/lang/String;)V", (void*) SetJsFlags },
    { "nativeRequestLabel", "(III)Ljava/lang/String;",
        (void*) RequestLabel },
    { "nativeRevealSelection", "(I)V", (void*) RevealSelection },
    { "nativeProvideVisitedHistory", "(I[Ljava/lang/String;)V",
        (void*) ProvideVisitedHistory },
    { "nativeFullScreenPluginHidden", "(II)V",
        (void*) FullScreenPluginHidden },
    { "nativePluginSurfaceReady", "(I)V",
        (void*) PluginSurfaceReady },
    { "nativeHitTest", "(IIIIZ)Landroid/webkit/WebViewCore$WebKitHitTest;",
        (void*) HitTest },
    { "nativeAutoFillForm", "(II)V",
        (void*) AutoFillForm },
    { "nativeScrollLayer", "(IILandroid/graphics/Rect;)V",
        (void*) ScrollRenderLayer },
//SAMSUNG : Reader >>
    { "applyreadability", "(ILjava/lang/String;)Ljava/lang/String;",
        (void*) applyreadability },
    { "loadinitialJs", "(ILjava/lang/String;)Ljava/lang/String;",
        (void*) loadinitialJs },
//SAMSUNG : Reader <<
//SAMSUNG : Copy image begin >>
    { "nativeSaveCachedImageToFile", "(Ljava/lang/String;Ljava/lang/String;)Z",
     (void*) SaveCachedImageToFile },
//SAMSUNG : Copy image end <<
//SAMSUNG ADVANCED TEXT SELECTION - BEGIN
    { "nativeWebTextSelectionAll", "(IIIII)V",
     (void*) WebTextSelectionAll },
    { "nativeCopyMoveSelection", "(IIIIZZFI)V",
     (void*) CopyMoveSelection },
    { "nativeClearTextSelectionEx", "(III)V",
     (void*) ClearTextSelection },
    { "nativeRecordSelectionCopiedData", "(ILandroid/graphics/Region;Landroid/graphics/Region;Landroid/graphics/Rect;Landroid/graphics/Rect;I)Z",
     (void*) RecordSelectionCopiedData },
    { "nativeGetSelectionGranularity", "(I)I",
     (void*) GetSelectionGranularity },
    { "nativeSelectClosestWord", "(IIIFZ)Z",
     (void*) SelectClosestWord },
    { "nativeGetSelectedText", "(I)Ljava/lang/String;",
        (void*) GetSelectedText },
// SAMSUNG CHANGE ++ GET_INPUT_TEXT_BOUNDS
    { "nativeGetInputTextBounds", "(ILandroid/graphics/Rect;)V",
        (void*) GetInputTextBounds },
// SAMSUNG CHANGE --
    { "nativeGetSelectionMultiColInfo", "(I)Z",
     (void*) GetSelectionMultiColInfo },
//SAMSUNG ADVANCED TEXT SELECTION - END
    { "nativeCloseIdleConnections", "(I)V",
        (void*) CloseIdleConnections },
//SAMSUNG CHANGES >>
    { "nativeGetWebFeedLinks", "(I)[Landroid/webkit/WebFeedLink;",
        (void*) nativeGetWebFeedLinks },
//SAMSUNG CHANGES <<
    { "nativeDeleteText", "(IIIII)V",
        (void*) DeleteText },
    { "nativeInsertText", "(ILjava/lang/String;)V",
        (void*) InsertText },
    { "nativeGetText", "(IIIII)Ljava/lang/String;",
        (void*) GetText },
    { "nativeSelectText", "(IIII)V",//4.2 Merge
        (void*) SelectText },
    { "nativeClearTextSelection", "(I)V",
        (void*) ClearSelection },
    { "nativeSelectWordAt", "(III)Z",
        (void*) SelectWordAt },
    { "nativeSelectAll", "(I)V",
        (void*) SelectAll },
    { "nativeCertTrustChanged","()V",
        (void*) nativeCertTrustChanged },
    { "nativeFindAll", "(ILjava/lang/String;)I",
        (void*) FindAll },
    { "nativeFindNext", "(IZ)I",
        (void*) FindNext },
	//SAMSUNG CHANGES >>> SPELLCHECK(sataya.m@samsung.com)
#if ENABLE(SPELLCHECK)
	{ "nativeCheckSpellingOfWordAtPosition", "(III)Z",
	(void*) nativeCheckSpellingOfWordAtPosition },
#endif
	//SAMSUNG CHANGES <<<	
    { "nativeSetInitialFocus", "(II)V", (void*) SetInitialFocus },
//SAMSUNG CHANGE HTML5 COLOR <<
    { "nativeSendColorPickerChoice", "(II)V",
        (void*) SendColorPickerChoice },
//SAMSUNG CHANGE HTML5 COLOR <<
//SAMSUNG CHANGE Form Navigation >>
    { "nativeMoveFocusToNext", "(I)Z",
        (void*) MoveFocusToNext },
    { "nativeMoveFocusToPrevious", "(I)Z",
        (void*) MoveFocusToPrevious },
//SAMSUNG CHANGE Form Navigation <<
// Samsung Change - HTML5 Web Notification	>>
    { "nativeNotificationPermissionsProvide", "(Ljava/lang/String;Z)V",
        (void*) NotificationPermissionsProvide },
    { "nativeNotificationResponseback", "(Ljava/lang/String;I)V",
        (void*) NotificationResponseback },
    { "nativeNotificationIDback", "(II)V",
        (void*) NotificationIDback },
// Samsung Change - HTML5 Web Notification	<<
//SAMSUNG CHANGES: mobile page zoom scale change issue - merge from ICS >>
    { "nativeRecalcWidthAndForceLayout", "(I)V",
        (void*) RecalcWidthAndForceLayout },
//SAMSUNG CHANGES <<
//SAMSUNG CHANGES MPSG100006129 >>
        { "nativeUpdateTextSelectionStartAndEnd", "(I)V",
            (void*) UpdateTextSelectionStartAndEnd },
//SAMSUNG CHANGES MPSG100006129 <<

// SAMSUNG CHANGES : SmartClip >>
    { "nativeExtractSmartClipData", "(IIIII)Ljava/lang/String;",
        (void*) nativeExtractSmartClipData },     
// SAMSUNG CHANGES : SmartClip <<

};

int registerWebViewCore(JNIEnv* env)
{
    jclass widget = env->FindClass("android/webkit/WebViewCore");
    ALOG_ASSERT(widget,
            "Unable to find class android/webkit/WebViewCore");
    gWebViewCoreFields.m_nativeClass = env->GetFieldID(widget, "mNativeClass",
            "I");
    ALOG_ASSERT(gWebViewCoreFields.m_nativeClass,
            "Unable to find android/webkit/WebViewCore.mNativeClass");
    gWebViewCoreFields.m_viewportWidth = env->GetFieldID(widget,
            "mViewportWidth", "I");
    ALOG_ASSERT(gWebViewCoreFields.m_viewportWidth,
            "Unable to find android/webkit/WebViewCore.mViewportWidth");
    gWebViewCoreFields.m_viewportHeight = env->GetFieldID(widget,
            "mViewportHeight", "I");
    ALOG_ASSERT(gWebViewCoreFields.m_viewportHeight,
            "Unable to find android/webkit/WebViewCore.mViewportHeight");
    gWebViewCoreFields.m_viewportInitialScale = env->GetFieldID(widget,
            "mViewportInitialScale", "I");
    ALOG_ASSERT(gWebViewCoreFields.m_viewportInitialScale,
            "Unable to find android/webkit/WebViewCore.mViewportInitialScale");
    gWebViewCoreFields.m_viewportMinimumScale = env->GetFieldID(widget,
            "mViewportMinimumScale", "I");
    ALOG_ASSERT(gWebViewCoreFields.m_viewportMinimumScale,
            "Unable to find android/webkit/WebViewCore.mViewportMinimumScale");
    gWebViewCoreFields.m_viewportMaximumScale = env->GetFieldID(widget,
            "mViewportMaximumScale", "I");
    ALOG_ASSERT(gWebViewCoreFields.m_viewportMaximumScale,
            "Unable to find android/webkit/WebViewCore.mViewportMaximumScale");
    gWebViewCoreFields.m_viewportUserScalable = env->GetFieldID(widget,
            "mViewportUserScalable", "Z");
    ALOG_ASSERT(gWebViewCoreFields.m_viewportUserScalable,
            "Unable to find android/webkit/WebViewCore.mViewportUserScalable");
    gWebViewCoreFields.m_viewportDensityDpi = env->GetFieldID(widget,
            "mViewportDensityDpi", "I");
    ALOG_ASSERT(gWebViewCoreFields.m_viewportDensityDpi,
            "Unable to find android/webkit/WebViewCore.mViewportDensityDpi");
    gWebViewCoreFields.m_drawIsPaused = env->GetFieldID(widget,
            "mDrawIsPaused", "Z");
    ALOG_ASSERT(gWebViewCoreFields.m_drawIsPaused,
            "Unable to find android/webkit/WebViewCore.mDrawIsPaused");
    gWebViewCoreFields.m_lowMemoryUsageMb = env->GetFieldID(widget, "mLowMemoryUsageThresholdMb", "I");
    gWebViewCoreFields.m_highMemoryUsageMb = env->GetFieldID(widget, "mHighMemoryUsageThresholdMb", "I");
    gWebViewCoreFields.m_highUsageDeltaMb = env->GetFieldID(widget, "mHighUsageDeltaMb", "I");

    gWebViewCoreStaticMethods.m_isSupportedMediaMimeType =
        env->GetStaticMethodID(widget, "isSupportedMediaMimeType", "(Ljava/lang/String;)Z");
    LOG_FATAL_IF(!gWebViewCoreStaticMethods.m_isSupportedMediaMimeType,
        "Could not find static method isSupportedMediaMimeType from WebViewCore");

    env->DeleteLocalRef(widget);

    return jniRegisterNativeMethods(env, "android/webkit/WebViewCore",
            gJavaWebViewCoreMethods, NELEM(gJavaWebViewCoreMethods));
}

} /* namespace android */
