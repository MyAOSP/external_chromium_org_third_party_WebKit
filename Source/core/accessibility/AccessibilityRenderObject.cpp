/*
* Copyright (C) 2008 Apple Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1.  Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
* 2.  Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
* 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
*     its contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "config.h"
#include "core/accessibility/AccessibilityRenderObject.h"

#include "bindings/v8/ExceptionStatePlaceholder.h"
#include "core/accessibility/AXObjectCache.h"
#include "core/accessibility/AccessibilityImageMapLink.h"
#include "core/accessibility/AccessibilitySVGRoot.h"
#include "core/accessibility/AccessibilitySpinButton.h"
#include "core/accessibility/AccessibilityTable.h"
#include "core/dom/NodeTraversal.h"
#include "core/editing/FrameSelection.h"
#include "core/editing/RenderedPosition.h"
#include "core/editing/VisibleUnits.h"
#include "core/editing/htmlediting.h"
#include "core/html/HTMLHtmlElement.h"
#include "core/html/HTMLImageElement.h"
#include "core/html/HTMLLabelElement.h"
#include "core/html/HTMLOptionElement.h"
#include "core/html/HTMLSelectElement.h"
#include "core/html/HTMLTextAreaElement.h"
#include "core/loader/ProgressTracker.h"
#include "core/page/Page.h"
#include "core/platform/LocalizedStrings.h"
#include "core/rendering/HitTestResult.h"
#include "core/rendering/RenderFieldset.h"
#include "core/rendering/RenderFileUploadControl.h"
#include "core/rendering/RenderHTMLCanvas.h"
#include "core/rendering/RenderImage.h"
#include "core/rendering/RenderInline.h"
#include "core/rendering/RenderLayer.h"
#include "core/rendering/RenderListMarker.h"
#include "core/rendering/RenderMenuList.h"
#include "core/rendering/RenderTextControlSingleLine.h"
#include "core/rendering/RenderTextFragment.h"
#include "core/rendering/RenderWidget.h"
#include "core/svg/SVGDocument.h"
#include "core/svg/SVGSVGElement.h"
#include "core/svg/graphics/SVGImage.h"
#include "wtf/StdLibExtras.h"

using namespace std;

namespace WebCore {

using namespace HTMLNames;

static inline RenderObject* firstChildInContinuation(RenderObject* renderer)
{
    RenderObject* r = toRenderInline(renderer)->continuation();

    while (r) {
        if (r->isRenderBlock())
            return r;
        if (RenderObject* child = r->firstChild())
            return child;
        r = toRenderInline(r)->continuation();
    }

    return 0;
}

static inline bool isInlineWithContinuation(RenderObject* object)
{
    if (!object->isBoxModelObject())
        return false;

    RenderBoxModelObject* renderer = toRenderBoxModelObject(object);
    if (!renderer->isRenderInline())
        return false;

    return toRenderInline(renderer)->continuation();
}

static inline RenderObject* firstChildConsideringContinuation(RenderObject* renderer)
{
    RenderObject* firstChild = renderer->firstChild();

    if (!firstChild && isInlineWithContinuation(renderer))
        firstChild = firstChildInContinuation(renderer);

    return firstChild;
}

static inline RenderInline* startOfContinuations(RenderObject* r)
{
    if (r->isInlineElementContinuation()) {
        return toRenderInline(r->node()->renderer());
    }

    // Blocks with a previous continuation always have a next continuation
    if (r->isRenderBlock() && toRenderBlock(r)->inlineElementContinuation())
        return toRenderInline(toRenderBlock(r)->inlineElementContinuation()->node()->renderer());

    return 0;
}

static inline RenderObject* endOfContinuations(RenderObject* renderer)
{
    RenderObject* prev = renderer;
    RenderObject* cur = renderer;

    if (!cur->isRenderInline() && !cur->isRenderBlock())
        return renderer;

    while (cur) {
        prev = cur;
        if (cur->isRenderInline()) {
            cur = toRenderInline(cur)->inlineElementContinuation();
            ASSERT(cur || !toRenderInline(prev)->continuation());
        } else
            cur = toRenderBlock(cur)->inlineElementContinuation();
    }

    return prev;
}

static inline RenderObject* childBeforeConsideringContinuations(RenderInline* r, RenderObject* child)
{
    RenderBoxModelObject* curContainer = r;
    RenderObject* cur = 0;
    RenderObject* prev = 0;

    while (curContainer) {
        if (curContainer->isRenderInline()) {
            cur = curContainer->firstChild();
            while (cur) {
                if (cur == child)
                    return prev;
                prev = cur;
                cur = cur->nextSibling();
            }

            curContainer = toRenderInline(curContainer)->continuation();
        } else if (curContainer->isRenderBlock()) {
            if (curContainer == child)
                return prev;

            prev = curContainer;
            curContainer = toRenderBlock(curContainer)->inlineElementContinuation();
        }
    }

    ASSERT_NOT_REACHED();

    return 0;
}

static inline bool firstChildIsInlineContinuation(RenderObject* renderer)
{
    return renderer->firstChild() && renderer->firstChild()->isInlineElementContinuation();
}

static inline bool lastChildHasContinuation(RenderObject* renderer)
{
    return renderer->lastChild() && isInlineWithContinuation(renderer->lastChild());
}

static RenderBoxModelObject* nextContinuation(RenderObject* renderer)
{
    ASSERT(renderer);
    if (renderer->isRenderInline() && !renderer->isReplaced())
        return toRenderInline(renderer)->continuation();
    if (renderer->isRenderBlock())
        return toRenderBlock(renderer)->inlineElementContinuation();
    return 0;
}

AccessibilityRenderObject::AccessibilityRenderObject(RenderObject* renderer)
    : AccessibilityNodeObject(renderer->node())
    , m_renderer(renderer)
    , m_cachedElementRectDirty(true)
{
#ifndef NDEBUG
    m_renderer->setHasAXObject(true);
#endif
}

PassRefPtr<AccessibilityRenderObject> AccessibilityRenderObject::create(RenderObject* renderer)
{
    return adoptRef(new AccessibilityRenderObject(renderer));
}

AccessibilityRenderObject::~AccessibilityRenderObject()
{
    ASSERT(isDetached());
}

LayoutRect AccessibilityRenderObject::elementRect() const
{
    if (!m_explicitElementRect.isEmpty())
        return m_explicitElementRect;
    if (!m_renderer)
        return LayoutRect();
    if (!m_renderer->isBox())
        return computeElementRect();

    for (const AccessibilityObject* obj = this; obj; obj = obj->parentObject()) {
        if (obj->isAccessibilityRenderObject())
            static_cast<const AccessibilityRenderObject*>(obj)->checkCachedElementRect();
    }
    for (const AccessibilityObject* obj = this; obj; obj = obj->parentObject()) {
        if (obj->isAccessibilityRenderObject())
            static_cast<const AccessibilityRenderObject*>(obj)->updateCachedElementRect();
    }

    return m_cachedElementRect;
}

void AccessibilityRenderObject::setRenderer(RenderObject* renderer)
{
    m_renderer = renderer;
    setNode(renderer->node());
}

RenderBoxModelObject* AccessibilityRenderObject::renderBoxModelObject() const
{
    if (!m_renderer || !m_renderer->isBoxModelObject())
        return 0;
    return toRenderBoxModelObject(m_renderer);
}

RenderView* AccessibilityRenderObject::topRenderer() const
{
    Document* topDoc = topDocument();
    if (!topDoc)
        return 0;

    return topDoc->renderView();
}

Document* AccessibilityRenderObject::topDocument() const
{
    if (!document())
        return 0;
    return document()->topDocument();
}

HTMLLabelElement* AccessibilityRenderObject::labelElementContainer() const
{
    if (!m_renderer)
        return 0;

    // the control element should not be considered part of the label
    if (isControl())
        return 0;

    // find if this has a parent that is a label
    for (Node* parentNode = m_renderer->node(); parentNode; parentNode = parentNode->parentNode()) {
        if (isHTMLLabelElement(parentNode))
            return toHTMLLabelElement(parentNode);
    }

    return 0;
}

bool AccessibilityRenderObject::shouldNotifyActiveDescendant() const
{
    // We want to notify that the combo box has changed its active descendant,
    // but we do not want to change the focus, because focus should remain with the combo box.
    if (isComboBox())
        return true;

    return shouldFocusActiveDescendant();
}

ScrollableArea* AccessibilityRenderObject::getScrollableAreaIfScrollable() const
{
    // If the parent is a scroll view, then this object isn't really scrollable, the parent ScrollView should handle the scrolling.
    if (parentObject() && parentObject()->isAccessibilityScrollView())
        return 0;

    if (!m_renderer || !m_renderer->isBox())
        return 0;

    RenderBox* box = toRenderBox(m_renderer);
    if (!box->canBeScrolledAndHasScrollableArea())
        return 0;

    return box->layer();
}

AccessibilityRole AccessibilityRenderObject::determineAccessibilityRole()
{
    if (!m_renderer)
        return UnknownRole;

    m_ariaRole = determineAriaRoleAttribute();

    Node* node = m_renderer->node();
    AccessibilityRole ariaRole = ariaRoleAttribute();
    if (ariaRole != UnknownRole)
        return ariaRole;

    RenderBoxModelObject* cssBox = renderBoxModelObject();

    if (node && node->isLink()) {
        if (cssBox && cssBox->isImage())
            return ImageMapRole;
        return WebCoreLinkRole;
    }
    if (cssBox && cssBox->isListItem())
        return ListItemRole;
    if (m_renderer->isListMarker())
        return ListMarkerRole;
    if (node && node->hasTagName(buttonTag))
        return buttonRoleType();
    if (node && node->hasTagName(legendTag))
        return LegendRole;
    if (m_renderer->isText())
        return StaticTextRole;
    if (cssBox && cssBox->isImage()) {
        if (node && node->hasTagName(inputTag))
            return ariaHasPopup() ? PopUpButtonRole : ButtonRole;
        if (isSVGImage())
            return SVGRootRole;
        return ImageRole;
    }

    // Note: if JavaScript is disabled, the renderer won't be a RenderHTMLCanvas.
    if (node && node->hasTagName(canvasTag) && m_renderer->isCanvas())
        return CanvasRole;

    if (cssBox && cssBox->isRenderView()) {
        // If the iframe is seamless, it should not be announced as a web area to AT clients.
        if (document() && document()->shouldDisplaySeamlesslyWithParent())
            return SeamlessWebAreaRole;
        return WebAreaRole;
    }

    if (cssBox && cssBox->isTextField())
        return TextFieldRole;

    if (cssBox && cssBox->isTextArea())
        return TextAreaRole;

    if (node && node->hasTagName(inputTag)) {
        HTMLInputElement* input = toHTMLInputElement(node);
        if (input->isCheckbox())
            return CheckBoxRole;
        if (input->isRadioButton())
            return RadioButtonRole;
        if (input->isTextButton())
            return buttonRoleType();

        const AtomicString& type = input->getAttribute(typeAttr);
        if (equalIgnoringCase(type, "color"))
            return ColorWellRole;
    }

    if (isFileUploadButton())
        return ButtonRole;

    if (cssBox && cssBox->isMenuList())
        return PopUpButtonRole;

    if (headingLevel())
        return HeadingRole;

    if (m_renderer->isSVGImage())
        return ImageRole;
    if (m_renderer->isSVGRoot())
        return SVGRootRole;

    if (node && node->hasTagName(ddTag))
        return DescriptionListDetailRole;

    if (node && node->hasTagName(dtTag))
        return DescriptionListTermRole;

    if (node && (node->hasTagName(rpTag) || node->hasTagName(rtTag)))
        return AnnotationRole;

    // Table sections should be ignored.
    if (m_renderer->isTableSection())
        return IgnoredRole;

    if (m_renderer->isHR())
        return HorizontalRuleRole;

    if (node && node->hasTagName(pTag))
        return ParagraphRole;

    if (node && isHTMLLabelElement(node))
        return LabelRole;

    if (node && node->hasTagName(divTag))
        return DivRole;

    if (node && node->hasTagName(formTag))
        return FormRole;

    if (node && node->hasTagName(articleTag))
        return DocumentArticleRole;

    if (node && node->hasTagName(mainTag))
        return LandmarkMainRole;

    if (node && node->hasTagName(navTag))
        return LandmarkNavigationRole;

    if (node && node->hasTagName(asideTag))
        return LandmarkComplementaryRole;

    if (node && node->hasTagName(sectionTag))
        return DocumentRegionRole;

    if (node && node->hasTagName(addressTag))
        return LandmarkContentInfoRole;

    // The HTML element should not be exposed as an element. That's what the RenderView element does.
    if (node && isHTMLHtmlElement(node))
        return IgnoredRole;

    // There should only be one banner/contentInfo per page. If header/footer are being used within an article or section
    // then it should not be exposed as whole page's banner/contentInfo
    if (node && node->hasTagName(headerTag) && !isDescendantOfElementType(articleTag) && !isDescendantOfElementType(sectionTag))
        return LandmarkBannerRole;
    if (node && node->hasTagName(footerTag) && !isDescendantOfElementType(articleTag) && !isDescendantOfElementType(sectionTag))
        return FooterRole;

    if (m_renderer->isBlockFlow())
        return GroupRole;

    // If the element does not have role, but it has ARIA attributes, accessibility should fallback to exposing it as a group.
    if (supportsARIAAttributes())
        return GroupRole;

    return UnknownRole;
}

void AccessibilityRenderObject::init()
{
    AccessibilityNodeObject::init();
}

void AccessibilityRenderObject::detach()
{
    AccessibilityNodeObject::detach();

    detachRemoteSVGRoot();

#ifndef NDEBUG
    if (m_renderer)
        m_renderer->setHasAXObject(false);
#endif
    m_renderer = 0;
}

//
// Check object role or purpose.
//

bool AccessibilityRenderObject::isAttachment() const
{
    RenderBoxModelObject* renderer = renderBoxModelObject();
    if (!renderer)
        return false;
    // Widgets are the replaced elements that we represent to AX as attachments
    bool isWidget = renderer->isWidget();
    ASSERT(!isWidget || (renderer->isReplaced() && !isImage()));
    return isWidget;
}

bool AccessibilityRenderObject::isFileUploadButton() const
{
    if (m_renderer && m_renderer->node() && m_renderer->node()->hasTagName(inputTag)) {
        HTMLInputElement* input = toHTMLInputElement(m_renderer->node());
        return input->isFileUpload();
    }

    return false;
}

static bool isLinkable(const AccessibilityObject& object)
{
    if (!object.renderer())
        return false;

    // See https://wiki.mozilla.org/Accessibility/AT-Windows-API for the elements
    // Mozilla considers linkable.
    return object.isLink() || object.isImage() || object.renderer()->isText();
}

bool AccessibilityRenderObject::isLinked() const
{
    if (!isLinkable(*this))
        return false;

    Element* anchor = anchorElement();
    if (!anchor || !isHTMLAnchorElement(anchor))
        return false;

    return !toHTMLAnchorElement(anchor)->href().isEmpty();
}

bool AccessibilityRenderObject::isLoaded() const
{
    return !m_renderer->document()->parser();
}

bool AccessibilityRenderObject::isOffScreen() const
{
    ASSERT(m_renderer);
    IntRect contentRect = pixelSnappedIntRect(m_renderer->absoluteClippedOverflowRect());
    FrameView* view = m_renderer->frame()->view();
    IntRect viewRect = view->visibleContentRect();
    viewRect.intersect(contentRect);
    return viewRect.isEmpty();
}

bool AccessibilityRenderObject::isReadOnly() const
{
    ASSERT(m_renderer);

    if (isWebArea()) {
        Document* document = m_renderer->document();
        if (!document)
            return true;

        HTMLElement* body = document->body();
        if (body && body->rendererIsEditable())
            return false;

        return !document->rendererIsEditable();
    }

    return AccessibilityNodeObject::isReadOnly();
}

bool AccessibilityRenderObject::isVisited() const
{
    // FIXME: Is it a privacy violation to expose visited information to accessibility APIs?
    return m_renderer->style()->isLink() && m_renderer->style()->insideLink() == InsideVisitedLink;
}

//
// Check object state.
//

bool AccessibilityRenderObject::isFocused() const
{
    if (!m_renderer)
        return false;

    Document* document = m_renderer->document();
    if (!document)
        return false;

    Element* focusedElement = document->focusedElement();
    if (!focusedElement)
        return false;

    // A web area is represented by the Document node in the DOM tree, which isn't focusable.
    // Check instead if the frame's selection controller is focused
    if (focusedElement == m_renderer->node()
        || (roleValue() == WebAreaRole && document->frame()->selection()->isFocusedAndActive()))
        return true;

    return false;
}

bool AccessibilityRenderObject::isSelected() const
{
    if (!m_renderer)
        return false;

    Node* node = m_renderer->node();
    if (!node)
        return false;

    const AtomicString& ariaSelected = getAttribute(aria_selectedAttr);
    if (equalIgnoringCase(ariaSelected, "true"))
        return true;

    if (isTabItem() && isTabItemSelected())
        return true;

    return false;
}

//
// Check whether certain properties can be modified.
//

bool AccessibilityRenderObject::canSetValueAttribute() const
{
    if (equalIgnoringCase(getAttribute(aria_readonlyAttr), "true"))
        return false;

    if (isProgressIndicator() || isSlider())
        return true;

    if (isTextControl() && !isNativeTextControl())
        return true;

    // Any node could be contenteditable, so isReadOnly should be relied upon
    // for this information for all elements.
    return !isReadOnly();
}

//
// Whether objects are ignored, i.e. not included in the tree.
//

AccessibilityObjectInclusion AccessibilityRenderObject::defaultObjectInclusion() const
{
    // The following cases can apply to any element that's a subclass of AccessibilityRenderObject.

    if (!m_renderer)
        return IgnoreObject;

    if (m_renderer->style()->visibility() != VISIBLE) {
        // aria-hidden is meant to override visibility as the determinant in AX hierarchy inclusion.
        if (equalIgnoringCase(getAttribute(aria_hiddenAttr), "false"))
            return DefaultBehavior;

        return IgnoreObject;
    }

    return AccessibilityObject::defaultObjectInclusion();
}

bool AccessibilityRenderObject::computeAccessibilityIsIgnored() const
{
#ifndef NDEBUG
    ASSERT(m_initialized);
#endif

    // Check first if any of the common reasons cause this element to be ignored.
    // Then process other use cases that need to be applied to all the various roles
    // that AccessibilityRenderObjects take on.
    AccessibilityObjectInclusion decision = defaultObjectInclusion();
    if (decision == IncludeObject)
        return false;
    if (decision == IgnoreObject)
        return true;

    // If this element is within a parent that cannot have children, it should not be exposed.
    if (isDescendantOfBarrenParent())
        return true;

    if (roleValue() == IgnoredRole)
        return true;

    if (roleValue() == PresentationalRole || inheritsPresentationalRole())
        return true;

    // An ARIA tree can only have tree items and static text as children.
    if (!isAllowedChildOfTree())
        return true;

    // TODO: we should refactor this - but right now this is necessary to make
    // sure scroll areas stay in the tree.
    if (isAttachment())
        return false;

    // ignore popup menu items because AppKit does
    for (RenderObject* parent = m_renderer->parent(); parent; parent = parent->parent()) {
        if (parent->isBoxModelObject() && toRenderBoxModelObject(parent)->isMenuList())
            return true;
    }

    // find out if this element is inside of a label element.
    // if so, it may be ignored because it's the label for a checkbox or radio button
    AccessibilityObject* controlObject = correspondingControlForLabelElement();
    if (controlObject && !controlObject->exposesTitleUIElement() && controlObject->isCheckboxOrRadio())
        return true;

    // NOTE: BRs always have text boxes now, so the text box check here can be removed
    if (m_renderer->isText()) {
        // static text beneath MenuItems and MenuButtons are just reported along with the menu item, so it's ignored on an individual level
        AccessibilityObject* parent = parentObjectUnignored();
        if (parent && (parent->ariaRoleAttribute() == MenuItemRole || parent->ariaRoleAttribute() == MenuButtonRole))
            return true;
        RenderText* renderText = toRenderText(m_renderer);
        if (m_renderer->isBR() || !renderText->firstTextBox())
            return true;

        // static text beneath TextControls is reported along with the text control text so it's ignored.
        for (AccessibilityObject* parent = parentObject(); parent; parent = parent->parentObject()) {
            if (parent->roleValue() == TextFieldRole)
                return true;
        }

        // text elements that are just empty whitespace should not be returned
        return renderText->text().impl()->containsOnlyWhitespace();
    }

    if (isHeading())
        return false;

    if (isLink())
        return false;

    // all controls are accessible
    if (isControl())
        return false;

    if (ariaRoleAttribute() != UnknownRole)
        return false;

    // don't ignore labels, because they serve as TitleUIElements
    Node* node = m_renderer->node();
    if (node && isHTMLLabelElement(node))
        return false;

    // Anything that is content editable should not be ignored.
    // However, one cannot just call node->rendererIsEditable() since that will ask if its parents
    // are also editable. Only the top level content editable region should be exposed.
    if (hasContentEditableAttributeSet())
        return false;

    // List items play an important role in defining the structure of lists. They should not be ignored.
    if (roleValue() == ListItemRole)
        return false;

    // if this element has aria attributes on it, it should not be ignored.
    if (supportsARIAAttributes())
        return false;

    // <span> tags are inline tags and not meant to convey information if they have no other aria
    // information on them. If we don't ignore them, they may emit signals expected to come from
    // their parent. In addition, because included spans are GroupRole objects, and GroupRole
    // objects are often containers with meaningful information, the inclusion of a span can have
    // the side effect of causing the immediate parent accessible to be ignored. This is especially
    // problematic for platforms which have distinct roles for textual block elements.
    if (node && node->hasTagName(spanTag))
        return true;

    if (m_renderer->isBlockFlow() && m_renderer->childrenInline() && !canSetFocusAttribute())
        return !toRenderBlock(m_renderer)->firstLineBox() && !mouseButtonListener();

    // ignore images seemingly used as spacers
    if (isImage()) {

        // If the image can take focus, it should not be ignored, lest the user not be able to interact with something important.
        if (canSetFocusAttribute())
            return false;

        if (node && node->isElementNode()) {
            Element* elt = toElement(node);
            const AtomicString& alt = elt->getAttribute(altAttr);
            // don't ignore an image that has an alt tag
            if (!alt.string().containsOnlyWhitespace())
                return false;
            // informal standard is to ignore images with zero-length alt strings
            if (!alt.isNull())
                return true;
        }

        if (isNativeImage()) {
            // check for one-dimensional image
            RenderImage* image = toRenderImage(m_renderer);
            if (image->height() <= 1 || image->width() <= 1)
                return true;

            // check whether rendered image was stretched from one-dimensional file image
            if (image->cachedImage()) {
                LayoutSize imageSize = image->cachedImage()->imageSizeForRenderer(m_renderer, image->view()->zoomFactor());
                return imageSize.height() <= 1 || imageSize.width() <= 1;
            }
        }
        return false;
    }

    if (isCanvas()) {
        if (canvasHasFallbackContent())
            return false;
        RenderHTMLCanvas* canvas = toRenderHTMLCanvas(m_renderer);
        if (canvas->height() <= 1 || canvas->width() <= 1)
            return true;
        // Otherwise fall through; use presence of help text, title, or description to decide.
    }

    if (isWebArea() || isSeamlessWebArea() || m_renderer->isListMarker())
        return false;

    // Using the help text, title or accessibility description (so we
    // check if there's some kind of accessible name for the element)
    // to decide an element's visibility is not as definitive as
    // previous checks, so this should remain as one of the last.
    //
    // These checks are simplified in the interest of execution speed;
    // for example, any element having an alt attribute will make it
    // not ignored, rather than just images.
    if (!getAttribute(aria_helpAttr).isEmpty() || !getAttribute(aria_describedbyAttr).isEmpty() || !getAttribute(altAttr).isEmpty() || !getAttribute(titleAttr).isEmpty())
        return false;

    // Don't ignore generic focusable elements like <div tabindex=0>
    // unless they're completely empty, with no children.
    if (isGenericFocusableElement() && node->firstChild())
        return false;

    if (!ariaAccessibilityDescription().isEmpty())
        return false;

    // By default, objects should be ignored so that the AX hierarchy is not
    // filled with unnecessary items.
    return true;
}

//
// Properties of static elements.
//

const AtomicString& AccessibilityRenderObject::accessKey() const
{
    Node* node = m_renderer->node();
    if (!node)
        return nullAtom;
    if (!node->isElementNode())
        return nullAtom;
    return toElement(node)->getAttribute(accesskeyAttr);
}

AccessibilityObject* AccessibilityRenderObject::correspondingControlForLabelElement() const
{
    HTMLLabelElement* labelElement = labelElementContainer();
    if (!labelElement)
        return 0;

    HTMLElement* correspondingControl = labelElement->control();
    if (!correspondingControl)
        return 0;

    // Make sure the corresponding control isn't a descendant of this label
    // that's in the middle of being destroyed.
    if (correspondingControl->renderer() && !correspondingControl->renderer()->parent())
        return 0;

    return axObjectCache()->getOrCreate(correspondingControl);
}

bool AccessibilityRenderObject::exposesTitleUIElement() const
{
    if (!isControl())
        return false;

    // If this control is ignored (because it's invisible),
    // then the label needs to be exposed so it can be visible to accessibility.
    if (accessibilityIsIgnored())
        return true;

    // Checkboxes and radio buttons use the text of their title ui element as their own AXTitle.
    // This code controls whether the title ui element should appear in the AX tree (usually, no).
    // It should appear if the control already has a label (which will be used as the AXTitle instead).
    if (isCheckboxOrRadio())
        return hasTextAlternative();

    // When controls have their own descriptions, the title element should be ignored.
    if (hasTextAlternative())
        return false;

    return true;
}

AccessibilityOrientation AccessibilityRenderObject::orientation() const
{
    const AtomicString& ariaOrientation = getAttribute(aria_orientationAttr);
    if (equalIgnoringCase(ariaOrientation, "horizontal"))
        return AccessibilityOrientationHorizontal;
    if (equalIgnoringCase(ariaOrientation, "vertical"))
        return AccessibilityOrientationVertical;

    return AccessibilityObject::orientation();
}

String AccessibilityRenderObject::text() const
{
    if (isPasswordField())
        return String();

    return AccessibilityNodeObject::text();
}

int AccessibilityRenderObject::textLength() const
{
    ASSERT(isTextControl());

    if (isPasswordField())
        return -1; // need to return something distinct from 0

    return text().length();
}

AccessibilityObject* AccessibilityRenderObject::titleUIElement() const
{
    if (!m_renderer)
        return 0;

    // if isFieldset is true, the renderer is guaranteed to be a RenderFieldset
    if (isFieldset())
        return axObjectCache()->getOrCreate(toRenderFieldset(m_renderer)->findLegend(RenderFieldset::IncludeFloatingOrOutOfFlow));

    Node* node = m_renderer->node();
    if (!node || !node->isElementNode())
        return 0;
    HTMLLabelElement* label = labelForElement(toElement(node));
    if (label && label->renderer())
        return axObjectCache()->getOrCreate(label);

    return 0;
}

KURL AccessibilityRenderObject::url() const
{
    if (isAnchor() && isHTMLAnchorElement(m_renderer->node())) {
        if (HTMLAnchorElement* anchor = toHTMLAnchorElement(anchorElement()))
            return anchor->href();
    }

    if (isWebArea())
        return m_renderer->document()->url();

    if (isImage() && m_renderer->node() && m_renderer->node()->hasTagName(imgTag))
        return toHTMLImageElement(m_renderer->node())->src();

    if (isInputImage())
        return toHTMLInputElement(m_renderer->node())->src();

    return KURL();
}

//
// Properties of interactive elements.
//

String AccessibilityRenderObject::actionVerb() const
{
    switch (roleValue()) {
    case ButtonRole:
    case ToggleButtonRole:
        return AXButtonActionVerb();
    case TextFieldRole:
    case TextAreaRole:
        return AXTextFieldActionVerb();
    case RadioButtonRole:
        return AXRadioButtonActionVerb();
    case CheckBoxRole:
        return isChecked() ? AXCheckedCheckBoxActionVerb() : AXUncheckedCheckBoxActionVerb();
    case LinkRole:
    case WebCoreLinkRole:
        return AXLinkActionVerb();
    default:
        return emptyString();
    }
}

void AccessibilityRenderObject::selectedChildren(AccessibilityChildrenVector& result)
{
    ASSERT(result.isEmpty());

    // only listboxes should be asked for their selected children.
    AccessibilityRole role = roleValue();
    if (role == ListBoxRole) // native list boxes would be AccessibilityListBoxes, so only check for aria list boxes
        ariaListboxSelectedChildren(result);
    else if (role == TreeRole || role == TreeGridRole || role == TableRole)
        ariaSelectedRows(result);
}

String AccessibilityRenderObject::stringValue() const
{
    if (!m_renderer)
        return String();

    if (isPasswordField())
        return String();

    RenderBoxModelObject* cssBox = renderBoxModelObject();

    if (ariaRoleAttribute() == StaticTextRole) {
        String staticText = text();
        if (!staticText.length())
            staticText = textUnderElement();
        return staticText;
    }

    if (m_renderer->isText())
        return textUnderElement();

    if (cssBox && cssBox->isMenuList()) {
        // RenderMenuList will go straight to the text() of its selected item.
        // This has to be overridden in the case where the selected item has an ARIA label.
        HTMLSelectElement* selectElement = toHTMLSelectElement(m_renderer->node());
        int selectedIndex = selectElement->selectedIndex();
        const Vector<HTMLElement*> listItems = selectElement->listItems();
        if (selectedIndex >= 0 && static_cast<size_t>(selectedIndex) < listItems.size()) {
            const AtomicString& overriddenDescription = listItems[selectedIndex]->fastGetAttribute(aria_labelAttr);
            if (!overriddenDescription.isNull())
                return overriddenDescription;
        }
        return toRenderMenuList(m_renderer)->text();
    }

    if (m_renderer->isListMarker())
        return toRenderListMarker(m_renderer)->text();

    if (isWebArea()) {
        // FIXME: Why would a renderer exist when the Document isn't attached to a frame?
        if (m_renderer->frame())
            return String();

        ASSERT_NOT_REACHED();
    }

    if (isTextControl())
        return text();

    if (m_renderer->isFileUploadControl())
        return toRenderFileUploadControl(m_renderer)->fileTextValue();

    // FIXME: We might need to implement a value here for more types
    // FIXME: It would be better not to advertise a value at all for the types for which we don't implement one;
    // this would require subclassing or making accessibilityAttributeNames do something other than return a
    // single static array.
    return String();
}

//
// ARIA attributes.
//

AccessibilityObject* AccessibilityRenderObject::activeDescendant() const
{
    if (!m_renderer)
        return 0;

    if (m_renderer->node() && !m_renderer->node()->isElementNode())
        return 0;
    Element* element = toElement(m_renderer->node());

    const AtomicString& activeDescendantAttrStr = element->getAttribute(aria_activedescendantAttr);
    if (activeDescendantAttrStr.isNull() || activeDescendantAttrStr.isEmpty())
        return 0;

    Element* target = element->treeScope()->getElementById(activeDescendantAttrStr);
    if (!target)
        return 0;

    AccessibilityObject* obj = axObjectCache()->getOrCreate(target);
    if (obj && obj->isAccessibilityRenderObject())
    // an activedescendant is only useful if it has a renderer, because that's what's needed to post the notification
        return obj;
    return 0;
}

void AccessibilityRenderObject::ariaFlowToElements(AccessibilityChildrenVector& flowTo) const
{
    Vector<Element*> elements;
    elementsFromAttribute(elements, aria_flowtoAttr);

    AXObjectCache* cache = axObjectCache();
    unsigned count = elements.size();
    for (unsigned k = 0; k < count; ++k) {
        Element* element = elements[k];
        AccessibilityObject* flowToElement = cache->getOrCreate(element);
        if (flowToElement)
            flowTo.append(flowToElement);
    }
}

bool AccessibilityRenderObject::ariaHasPopup() const
{
    return elementAttributeValue(aria_haspopupAttr);
}

bool AccessibilityRenderObject::ariaRoleHasPresentationalChildren() const
{
    switch (m_ariaRole) {
    case ButtonRole:
    case SliderRole:
    case ImageRole:
    case ProgressIndicatorRole:
    case SpinButtonRole:
    // case SeparatorRole:
        return true;
    default:
        return false;
    }
}

bool AccessibilityRenderObject::isPresentationalChildOfAriaRole() const
{
    // Walk the parent chain looking for a parent that has presentational children
    AccessibilityObject* parent;
    for (parent = parentObject(); parent && !parent->ariaRoleHasPresentationalChildren(); parent = parent->parentObject())
    { }

    return parent;
}

bool AccessibilityRenderObject::shouldFocusActiveDescendant() const
{
    switch (ariaRoleAttribute()) {
    case GroupRole:
    case ListBoxRole:
    case MenuRole:
    case MenuBarRole:
    case RadioGroupRole:
    case RowRole:
    case PopUpButtonRole:
    case ProgressIndicatorRole:
    case ToolbarRole:
    case OutlineRole:
    case TreeRole:
    case GridRole:
    /* FIXME: replace these with actual roles when they are added to AccessibilityRole
    composite
    alert
    alertdialog
    status
    timer
    */
        return true;
    default:
        return false;
    }
}

bool AccessibilityRenderObject::supportsARIADragging() const
{
    const AtomicString& grabbed = getAttribute(aria_grabbedAttr);
    return equalIgnoringCase(grabbed, "true") || equalIgnoringCase(grabbed, "false");
}

bool AccessibilityRenderObject::supportsARIADropping() const
{
    const AtomicString& dropEffect = getAttribute(aria_dropeffectAttr);
    return !dropEffect.isEmpty();
}

bool AccessibilityRenderObject::supportsARIAFlowTo() const
{
    return !getAttribute(aria_flowtoAttr).isEmpty();
}

bool AccessibilityRenderObject::supportsARIAOwns() const
{
    if (!m_renderer)
        return false;
    const AtomicString& ariaOwns = getAttribute(aria_ownsAttr);

    return !ariaOwns.isEmpty();
}

//
// ARIA live-region features.
//

const AtomicString& AccessibilityRenderObject::ariaLiveRegionStatus() const
{
    DEFINE_STATIC_LOCAL(const AtomicString, liveRegionStatusAssertive, ("assertive", AtomicString::ConstructFromLiteral));
    DEFINE_STATIC_LOCAL(const AtomicString, liveRegionStatusPolite, ("polite", AtomicString::ConstructFromLiteral));
    DEFINE_STATIC_LOCAL(const AtomicString, liveRegionStatusOff, ("off", AtomicString::ConstructFromLiteral));

    const AtomicString& liveRegionStatus = getAttribute(aria_liveAttr);
    // These roles have implicit live region status.
    if (liveRegionStatus.isEmpty()) {
        switch (roleValue()) {
        case ApplicationAlertDialogRole:
        case ApplicationAlertRole:
            return liveRegionStatusAssertive;
        case ApplicationLogRole:
        case ApplicationStatusRole:
            return liveRegionStatusPolite;
        case ApplicationTimerRole:
        case ApplicationMarqueeRole:
            return liveRegionStatusOff;
        default:
            break;
        }
    }

    return liveRegionStatus;
}

const AtomicString& AccessibilityRenderObject::ariaLiveRegionRelevant() const
{
    DEFINE_STATIC_LOCAL(const AtomicString, defaultLiveRegionRelevant, ("additions text", AtomicString::ConstructFromLiteral));
    const AtomicString& relevant = getAttribute(aria_relevantAttr);

    // Default aria-relevant = "additions text".
    if (relevant.isEmpty())
        return defaultLiveRegionRelevant;

    return relevant;
}

bool AccessibilityRenderObject::ariaLiveRegionAtomic() const
{
    return elementAttributeValue(aria_atomicAttr);
}

bool AccessibilityRenderObject::ariaLiveRegionBusy() const
{
    return elementAttributeValue(aria_busyAttr);
}

//
// Accessibility Text.
//

String AccessibilityRenderObject::textUnderElement() const
{
    if (!m_renderer)
        return String();

    if (m_renderer->isFileUploadControl())
        return toRenderFileUploadControl(m_renderer)->buttonValue();

    if (m_renderer->isText()) {
        // If possible, use a text iterator to get the text, so that whitespace
        // is handled consistently.
        if (Node* node = this->node()) {
            if (Frame* frame = node->document()->frame()) {
                // catch stale WebCoreAXObject (see <rdar://problem/3960196>)
                if (frame->document() != node->document())
                    return String();

                return plainText(rangeOfContents(node).get(), textIteratorBehaviorForTextRange());
            }
        }

        // Sometimes text fragments don't have Nodes associated with them (like when
        // CSS content is used to insert text or when a RenderCounter is used.)
        RenderText* renderTextObject = toRenderText(m_renderer);
        if (renderTextObject->isTextFragment())
            return String(static_cast<RenderTextFragment*>(m_renderer)->contentString());
        else
            return String(renderTextObject->text());
    }

    return AccessibilityNodeObject::textUnderElement();
}

//
// Accessibility Text - (To be deprecated).
//

String AccessibilityRenderObject::helpText() const
{
    if (!m_renderer)
        return String();

    const AtomicString& ariaHelp = getAttribute(aria_helpAttr);
    if (!ariaHelp.isEmpty())
        return ariaHelp;

    String describedBy = ariaDescribedByAttribute();
    if (!describedBy.isEmpty())
        return describedBy;

    String description = accessibilityDescription();
    for (RenderObject* curr = m_renderer; curr; curr = curr->parent()) {
        if (curr->node() && curr->node()->isHTMLElement()) {
            const AtomicString& summary = toElement(curr->node())->getAttribute(summaryAttr);
            if (!summary.isEmpty())
                return summary;

            // The title attribute should be used as help text unless it is already being used as descriptive text.
            const AtomicString& title = toElement(curr->node())->getAttribute(titleAttr);
            if (!title.isEmpty() && description != title)
                return title;
        }

        // Only take help text from an ancestor element if its a group or an unknown role. If help was
        // added to those kinds of elements, it is likely it was meant for a child element.
        AccessibilityObject* axObj = axObjectCache()->getOrCreate(curr);
        if (axObj) {
            AccessibilityRole role = axObj->roleValue();
            if (role != GroupRole && role != UnknownRole)
                break;
        }
    }

    return String();
}

//
// Position and size.
//

void AccessibilityRenderObject::checkCachedElementRect() const
{
    if (m_cachedElementRectDirty)
        return;

    if (!m_renderer)
        return;

    if (!m_renderer->isBox())
        return;

    bool dirty = false;
    RenderBox* box = toRenderBox(m_renderer);
    if (box->frameRect() != m_cachedFrameRect)
        dirty = true;

    if (box->canBeScrolledAndHasScrollableArea()) {
        ScrollableArea* scrollableArea = box->layer();
        if (scrollableArea && scrollableArea->scrollPosition() != m_cachedScrollPosition)
            dirty = true;
    }

    if (dirty)
        markCachedElementRectDirty();
}

void AccessibilityRenderObject::updateCachedElementRect() const
{
    if (!m_cachedElementRectDirty)
        return;

    if (!m_renderer)
        return;

    if (!m_renderer->isBox())
        return;

    RenderBox* box = toRenderBox(m_renderer);
    m_cachedFrameRect = box->frameRect();

    if (box->canBeScrolledAndHasScrollableArea()) {
        ScrollableArea* scrollableArea = box->layer();
        if (scrollableArea)
            m_cachedScrollPosition = scrollableArea->scrollPosition();
    }

    m_cachedElementRect = computeElementRect();
    m_cachedElementRectDirty = false;
}

void AccessibilityRenderObject::markCachedElementRectDirty() const
{
    if (m_cachedElementRectDirty)
        return;

    // Marks children recursively, if this element changed.
    m_cachedElementRectDirty = true;
    for (AccessibilityObject* child = firstChild(); child; child = child->nextSibling())
        child->markCachedElementRectDirty();
}

IntPoint AccessibilityRenderObject::clickPoint()
{
    // Headings are usually much wider than their textual content. If the mid point is used, often it can be wrong.
    if (isHeading() && children().size() == 1)
        return children()[0]->clickPoint();

    // use the default position unless this is an editable web area, in which case we use the selection bounds.
    if (!isWebArea() || isReadOnly())
        return AccessibilityObject::clickPoint();

    LayoutRect bounds = elementRect();
    return IntPoint(bounds.x() + (bounds.width() / 2), bounds.y() - (bounds.height() / 2));
}

//
// Hit testing.
//

AccessibilityObject* AccessibilityRenderObject::accessibilityHitTest(const IntPoint& point) const
{
    if (!m_renderer || !m_renderer->hasLayer())
        return 0;

    RenderLayer* layer = toRenderBox(m_renderer)->layer();

    HitTestRequest request(HitTestRequest::ReadOnly | HitTestRequest::Active);
    HitTestResult hitTestResult = HitTestResult(point);
    layer->hitTest(request, hitTestResult);
    if (!hitTestResult.innerNode())
        return 0;
    Node* node = hitTestResult.innerNode()->deprecatedShadowAncestorNode();

    if (isHTMLAreaElement(node))
        return accessibilityImageMapHitTest(toHTMLAreaElement(node), point);

    if (node->hasTagName(optionTag))
        node = toHTMLOptionElement(node)->ownerSelectElement();

    RenderObject* obj = node->renderer();
    if (!obj)
        return 0;

    AccessibilityObject* result = obj->document()->axObjectCache()->getOrCreate(obj);
    result->updateChildrenIfNecessary();

    // Allow the element to perform any hit-testing it might need to do to reach non-render children.
    result = result->elementAccessibilityHitTest(point);

    if (result && result->accessibilityIsIgnored()) {
        // If this element is the label of a control, a hit test should return the control.
        if (result->isAccessibilityRenderObject()) {
            AccessibilityObject* controlObject = static_cast<AccessibilityRenderObject*>(result)->correspondingControlForLabelElement();
            if (controlObject && !controlObject->exposesTitleUIElement())
                return controlObject;
        }

        result = result->parentObjectUnignored();
    }

    return result;
}

AccessibilityObject* AccessibilityRenderObject::elementAccessibilityHitTest(const IntPoint& point) const
{
    if (isSVGImage())
        return remoteSVGElementHitTest(point);

    return AccessibilityObject::elementAccessibilityHitTest(point);
}

//
// High-level accessibility tree access.
//

AccessibilityObject* AccessibilityRenderObject::parentObject() const
{
    if (!m_renderer)
        return 0;

    if (ariaRoleAttribute() == MenuBarRole)
        return axObjectCache()->getOrCreate(m_renderer->parent());

    // menuButton and its corresponding menu are DOM siblings, but Accessibility needs them to be parent/child
    if (ariaRoleAttribute() == MenuRole) {
        AccessibilityObject* parent = menuButtonForMenu();
        if (parent)
            return parent;
    }

    RenderObject* parentObj = renderParentObject();
    if (parentObj)
        return axObjectCache()->getOrCreate(parentObj);

    // WebArea's parent should be the scroll view containing it.
    if (isWebArea() || isSeamlessWebArea())
        return axObjectCache()->getOrCreate(m_renderer->frame()->view());

    return 0;
}

AccessibilityObject* AccessibilityRenderObject::parentObjectIfExists() const
{
    // WebArea's parent should be the scroll view containing it.
    if (isWebArea() || isSeamlessWebArea())
        return axObjectCache()->get(m_renderer->frame()->view());

    return axObjectCache()->get(renderParentObject());
}

//
// Low-level accessibility tree exploration, only for use within the accessibility module.
//

AccessibilityObject* AccessibilityRenderObject::firstChild() const
{
    if (!m_renderer)
        return 0;

    RenderObject* firstChild = firstChildConsideringContinuation(m_renderer);

    if (!firstChild)
        return 0;

    return axObjectCache()->getOrCreate(firstChild);
}

AccessibilityObject* AccessibilityRenderObject::nextSibling() const
{
    if (!m_renderer)
        return 0;

    RenderObject* nextSibling = 0;

    // Case 1: node is a block and has an inline continuation. Next sibling is the inline continuation's
    // first child.
    RenderInline* inlineContinuation;
    if (m_renderer->isRenderBlock() && (inlineContinuation = toRenderBlock(m_renderer)->inlineElementContinuation()))
        nextSibling = firstChildConsideringContinuation(inlineContinuation);

    // Case 2: Anonymous block parent of the start of a continuation - skip all the way to
    // after the parent of the end, since everything in between will be linked up via the continuation.
    else if (m_renderer->isAnonymousBlock() && lastChildHasContinuation(m_renderer)) {
        RenderObject* lastParent = endOfContinuations(m_renderer->lastChild())->parent();
        while (lastChildHasContinuation(lastParent))
            lastParent = endOfContinuations(lastParent->lastChild())->parent();
        nextSibling = lastParent->nextSibling();
    }

    // Case 3: node has an actual next sibling
    else if (RenderObject* ns = m_renderer->nextSibling())
        nextSibling = ns;

    // Case 4: node is an inline with a continuation. Next sibling is the next sibling of the end
    // of the continuation chain.
    else if (isInlineWithContinuation(m_renderer))
        nextSibling = endOfContinuations(m_renderer)->nextSibling();

    // Case 5: node has no next sibling, and its parent is an inline with a continuation.
    else if (isInlineWithContinuation(m_renderer->parent())) {
        RenderObject* continuation = toRenderInline(m_renderer->parent())->continuation();

        // Case 5a: continuation is a block - in this case the block itself is the next sibling.
        if (continuation->isRenderBlock())
            nextSibling = continuation;
        // Case 5b: continuation is an inline - in this case the inline's first child is the next sibling
        else
            nextSibling = firstChildConsideringContinuation(continuation);
    }

    if (!nextSibling)
        return 0;

    return axObjectCache()->getOrCreate(nextSibling);
}

void AccessibilityRenderObject::addChildren()
{
    // If the need to add more children in addition to existing children arises,
    // childrenChanged should have been called, leaving the object with no children.
    ASSERT(!m_haveChildren);

    m_haveChildren = true;

    if (!canHaveChildren())
        return;

    for (RefPtr<AccessibilityObject> obj = firstChild(); obj; obj = obj->nextSibling())
        addChild(obj.get());

    addHiddenChildren();
    addAttachmentChildren();
    addImageMapChildren();
    addTextFieldChildren();
    addCanvasChildren();
    addRemoteSVGChildren();
}

bool AccessibilityRenderObject::canHaveChildren() const
{
    if (!m_renderer)
        return false;

    return AccessibilityNodeObject::canHaveChildren();
}

void AccessibilityRenderObject::updateChildrenIfNecessary()
{
    if (needsToUpdateChildren())
        clearChildren();

    AccessibilityObject::updateChildrenIfNecessary();
}

void AccessibilityRenderObject::clearChildren()
{
    AccessibilityObject::clearChildren();
    m_childrenDirty = false;
}

AccessibilityObject* AccessibilityRenderObject::observableObject() const
{
    // Find the object going up the parent chain that is used in accessibility to monitor certain notifications.
    for (RenderObject* renderer = m_renderer; renderer && renderer->node(); renderer = renderer->parent()) {
        if (renderObjectIsObservable(renderer))
            return axObjectCache()->getOrCreate(renderer);
    }

    return 0;
}

//
// Properties of the object's owning document or page.
//

double AccessibilityRenderObject::estimatedLoadingProgress() const
{
    if (!m_renderer)
        return 0;

    if (isLoaded())
        return 1.0;

    Page* page = m_renderer->document()->page();
    if (!page)
        return 0;

    return page->progress()->estimatedProgress();
}

//
// DOM and Render tree access.
//

Node* AccessibilityRenderObject::node() const
{
    return m_renderer ? m_renderer->node() : 0;
}

Document* AccessibilityRenderObject::document() const
{
    if (!m_renderer)
        return 0;
    return m_renderer->document();
}

FrameView* AccessibilityRenderObject::documentFrameView() const
{
    if (!m_renderer || !m_renderer->document())
        return 0;

    // this is the RenderObject's Document's Frame's FrameView
    return m_renderer->document()->view();
}

Element* AccessibilityRenderObject::anchorElement() const
{
    if (!m_renderer)
        return 0;

    AXObjectCache* cache = axObjectCache();
    RenderObject* currRenderer;

    // Search up the render tree for a RenderObject with a DOM node.  Defer to an earlier continuation, though.
    for (currRenderer = m_renderer; currRenderer && !currRenderer->node(); currRenderer = currRenderer->parent()) {
        if (currRenderer->isAnonymousBlock()) {
            RenderObject* continuation = toRenderBlock(currRenderer)->continuation();
            if (continuation)
                return cache->getOrCreate(continuation)->anchorElement();
        }
    }

    // bail if none found
    if (!currRenderer)
        return 0;

    // search up the DOM tree for an anchor element
    // NOTE: this assumes that any non-image with an anchor is an HTMLAnchorElement
    Node* node = currRenderer->node();
    for ( ; node; node = node->parentNode()) {
        if (isHTMLAnchorElement(node) || (node->renderer() && cache->getOrCreate(node->renderer())->isAnchor()))
            return toElement(node);
    }

    return 0;
}

Widget* AccessibilityRenderObject::widgetForAttachmentView() const
{
    if (!isAttachment())
        return 0;
    return toRenderWidget(m_renderer)->widget();
}

//
// Selected text.
//

PlainTextRange AccessibilityRenderObject::selectedTextRange() const
{
    ASSERT(isTextControl());

    if (isPasswordField())
        return PlainTextRange();

    AccessibilityRole ariaRole = ariaRoleAttribute();
    if (isNativeTextControl() && ariaRole == UnknownRole) {
        HTMLTextFormControlElement* textControl = toRenderTextControl(m_renderer)->textFormControlElement();
        return PlainTextRange(textControl->selectionStart(), textControl->selectionEnd() - textControl->selectionStart());
    }

    if (ariaRole == UnknownRole)
        return PlainTextRange();

    return ariaSelectedTextRange();
}

VisibleSelection AccessibilityRenderObject::selection() const
{
    return m_renderer->frame()->selection()->selection();
}

String AccessibilityRenderObject::selectedText() const
{
    ASSERT(isTextControl());

    if (isPasswordField())
        return String(); // need to return something distinct from empty string

    if (isNativeTextControl()) {
        HTMLTextFormControlElement* textControl = toRenderTextControl(m_renderer)->textFormControlElement();
        return textControl->selectedText();
    }

    if (ariaRoleAttribute() == UnknownRole)
        return String();

    return stringForRange(ariaSelectedTextRange());
}

//
// Modify or take an action on an object.
//

void AccessibilityRenderObject::setFocused(bool on)
{
    if (!canSetFocusAttribute())
        return;

    Document* document = this->document();
    if (!on)
        document->setFocusedElement(0);
    else {
        Node* node = this->node();
        if (node && node->isElementNode()) {
            // If this node is already the currently focused node, then calling focus() won't do anything.
            // That is a problem when focus is removed from the webpage to chrome, and then returns.
            // In these cases, we need to do what keyboard and mouse focus do, which is reset focus first.
            if (document->focusedElement() == node)
                document->setFocusedElement(0);

            toElement(node)->focus();
        } else {
            document->setFocusedElement(0);
        }
    }
}

void AccessibilityRenderObject::setSelectedTextRange(const PlainTextRange& range)
{
    if (isNativeTextControl()) {
        HTMLTextFormControlElement* textControl = toRenderTextControl(m_renderer)->textFormControlElement();
        textControl->setSelectionRange(range.start, range.start + range.length);
        return;
    }

    Document* document = m_renderer->document();
    if (!document)
        return;
    Frame* frame = document->frame();
    if (!frame)
        return;
    Node* node = m_renderer->node();
    frame->selection()->setSelection(VisibleSelection(Position(node, range.start, Position::PositionIsOffsetInAnchor),
        Position(node, range.start + range.length, Position::PositionIsOffsetInAnchor), DOWNSTREAM));
}

void AccessibilityRenderObject::setValue(const String& string)
{
    if (!m_renderer || !m_renderer->node() || !m_renderer->node()->isElementNode())
        return;
    if (!m_renderer->isBoxModelObject())
        return;
    RenderBoxModelObject* renderer = toRenderBoxModelObject(m_renderer);

    // FIXME: Do we want to do anything here for ARIA textboxes?
    if (renderer->isTextField()) {
        // FIXME: This is not safe!  Other elements could have a TextField renderer.
        toHTMLInputElement(m_renderer->node())->setValue(string);
    } else if (renderer->isTextArea()) {
        // FIXME: This is not safe!  Other elements could have a TextArea renderer.
        toHTMLTextAreaElement(m_renderer->node())->setValue(string);
    }
}

void AccessibilityRenderObject::scrollTo(const IntPoint& point) const
{
    if (!m_renderer || !m_renderer->isBox())
        return;

    RenderBox* box = toRenderBox(m_renderer);
    if (!box->canBeScrolledAndHasScrollableArea())
        return;

    RenderLayer* layer = box->layer();
    layer->scrollToOffset(toIntSize(point), RenderLayer::ScrollOffsetClamped);
}

//
// Notifications that this object may have changed.
//

void AccessibilityRenderObject::handleActiveDescendantChanged()
{
    Element* element = toElement(renderer()->node());
    if (!element)
        return;
    Document* doc = renderer()->document();
    if (!doc->frame()->selection()->isFocusedAndActive() || doc->focusedElement() != element)
        return;
    AccessibilityRenderObject* activedescendant = static_cast<AccessibilityRenderObject*>(activeDescendant());

    if (activedescendant && shouldNotifyActiveDescendant())
        doc->axObjectCache()->postNotification(m_renderer, AXObjectCache::AXActiveDescendantChanged, true);
}

void AccessibilityRenderObject::handleAriaExpandedChanged()
{
    // Find if a parent of this object should handle aria-expanded changes.
    AccessibilityObject* containerParent = this->parentObject();
    while (containerParent) {
        bool foundParent = false;

        switch (containerParent->roleValue()) {
        case TreeRole:
        case TreeGridRole:
        case GridRole:
        case TableRole:
        case BrowserRole:
            foundParent = true;
            break;
        default:
            break;
        }

        if (foundParent)
            break;

        containerParent = containerParent->parentObject();
    }

    // Post that the row count changed.
    if (containerParent)
        axObjectCache()->postNotification(containerParent, document(), AXObjectCache::AXRowCountChanged, true);

    // Post that the specific row either collapsed or expanded.
    if (roleValue() == RowRole || roleValue() == TreeItemRole)
        axObjectCache()->postNotification(this, document(), isExpanded() ? AXObjectCache::AXRowExpanded : AXObjectCache::AXRowCollapsed, true);
}

//
// Text metrics. Most of these should be deprecated, needs major cleanup.
//

// NOTE: Consider providing this utility method as AX API
int AccessibilityRenderObject::index(const VisiblePosition& position) const
{
    if (position.isNull() || !isTextControl())
        return -1;

    if (renderObjectContainsPosition(m_renderer, position.deepEquivalent()))
        return indexForVisiblePosition(position);

    return -1;
}

VisiblePosition AccessibilityRenderObject::visiblePositionForIndex(int index) const
{
    if (!m_renderer)
        return VisiblePosition();

    if (isNativeTextControl())
        return toRenderTextControl(m_renderer)->textFormControlElement()->visiblePositionForIndex(index);

    if (!allowsTextRanges() && !m_renderer->isText())
        return VisiblePosition();

    Node* node = m_renderer->node();
    if (!node)
        return VisiblePosition();

    if (index <= 0)
        return VisiblePosition(firstPositionInOrBeforeNode(node), DOWNSTREAM);

    RefPtr<Range> range = Range::create(m_renderer->document());
    range->selectNodeContents(node, IGNORE_EXCEPTION);
    CharacterIterator it(range.get());
    it.advance(index - 1);
    return VisiblePosition(Position(it.range()->endContainer(), it.range()->endOffset(), Position::PositionIsOffsetInAnch\
or), UPSTREAM);
}

int AccessibilityRenderObject::indexForVisiblePosition(const VisiblePosition& pos) const
{
    if (isNativeTextControl()) {
        HTMLTextFormControlElement* textControl = toRenderTextControl(m_renderer)->textFormControlElement();
        return textControl->indexForVisiblePosition(pos);
    }

    if (!isTextControl())
        return 0;

    Node* node = m_renderer->node();
    if (!node)
        return 0;

    Position indexPosition = pos.deepEquivalent();
    if (indexPosition.isNull() || highestEditableRoot(indexPosition, HasEditableAXRole) != node)
        return 0;

    RefPtr<Range> range = Range::create(m_renderer->document());
    range->setStart(node, 0, IGNORE_EXCEPTION);
    range->setEnd(indexPosition, IGNORE_EXCEPTION);

    return TextIterator::rangeLength(range.get());
}

void AccessibilityRenderObject::lineBreaks(Vector<int>& lineBreaks) const
{
    if (!isTextControl())
        return;

    VisiblePosition visiblePos = visiblePositionForIndex(0);
    VisiblePosition savedVisiblePos = visiblePos;
    visiblePos = nextLinePosition(visiblePos, 0);
    while (!visiblePos.isNull() && visiblePos != savedVisiblePos) {
        lineBreaks.append(indexForVisiblePosition(visiblePos));
        savedVisiblePos = visiblePos;
        visiblePos = nextLinePosition(visiblePos, 0);
    }
}

// A substring of the text associated with this accessibility object that is
// specified by the given character range.
String AccessibilityRenderObject::stringForRange(const PlainTextRange& range) const
{
    if (!range.length)
        return String();

    if (!isTextControl())
        return String();

    String elementText = isPasswordField() ? String() : text();
    if (range.start + range.length > elementText.length())
        return String();

    return elementText.substring(range.start, range.length);
}

//
// Private.
//

bool AccessibilityRenderObject::isAllowedChildOfTree() const
{
    // Determine if this is in a tree. If so, we apply special behavior to make it work like an AXOutline.
    AccessibilityObject* axObj = parentObject();
    bool isInTree = false;
    while (axObj) {
        if (axObj->isTree()) {
            isInTree = true;
            break;
        }
        axObj = axObj->parentObject();
    }

    // If the object is in a tree, only tree items should be exposed (and the children of tree items).
    if (isInTree) {
        AccessibilityRole role = roleValue();
        if (role != TreeItemRole && role != StaticTextRole)
            return false;
    }
    return true;
}

bool AccessibilityRenderObject::hasTextAlternative() const
{
    // ARIA: section 2A, bullet #3 says if aria-labeledby or aria-label appears, it should
    // override the "label" element association.
    if (!ariaLabeledByAttribute().isEmpty() || !getAttribute(aria_labelAttr).isEmpty())
        return true;

    return false;
}

void AccessibilityRenderObject::ariaListboxSelectedChildren(AccessibilityChildrenVector& result)
{
    bool isMulti = isMultiSelectable();

    AccessibilityChildrenVector childObjects = children();
    unsigned childrenSize = childObjects.size();
    for (unsigned k = 0; k < childrenSize; ++k) {
        // Every child should have aria-role option, and if so, check for selected attribute/state.
        AccessibilityObject* child = childObjects[k].get();
        if (child->isSelected() && child->ariaRoleAttribute() == ListBoxOptionRole) {
            result.append(child);
            if (!isMulti)
                return;
        }
    }
}

PlainTextRange AccessibilityRenderObject::ariaSelectedTextRange() const
{
    Node* node = m_renderer->node();
    if (!node)
        return PlainTextRange();

    VisibleSelection visibleSelection = selection();
    RefPtr<Range> currentSelectionRange = visibleSelection.toNormalizedRange();
    if (!currentSelectionRange || !currentSelectionRange->intersectsNode(node, IGNORE_EXCEPTION))
        return PlainTextRange();

    int start = indexForVisiblePosition(visibleSelection.start());
    int end = indexForVisiblePosition(visibleSelection.end());

    return PlainTextRange(start, end - start);
}

bool AccessibilityRenderObject::nodeIsTextControl(const Node* node) const
{
    if (!node)
        return false;

    const AccessibilityObject* axObjectForNode = axObjectCache()->getOrCreate(const_cast<Node*>(node));
    if (!axObjectForNode)
        return false;

    return axObjectForNode->isTextControl();
}

bool AccessibilityRenderObject::isTabItemSelected() const
{
    if (!isTabItem() || !m_renderer)
        return false;

    Node* node = m_renderer->node();
    if (!node || !node->isElementNode())
        return false;

    // The ARIA spec says a tab item can also be selected if it is aria-labeled by a tabpanel
    // that has keyboard focus inside of it, or if a tabpanel in its aria-controls list has KB
    // focus inside of it.
    AccessibilityObject* focusedElement = focusedUIElement();
    if (!focusedElement)
        return false;

    Vector<Element*> elements;
    elementsFromAttribute(elements, aria_controlsAttr);

    unsigned count = elements.size();
    for (unsigned k = 0; k < count; ++k) {
        Element* element = elements[k];
        AccessibilityObject* tabPanel = axObjectCache()->getOrCreate(element);

        // A tab item should only control tab panels.
        if (!tabPanel || tabPanel->roleValue() != TabPanelRole)
            continue;

        AccessibilityObject* checkFocusElement = focusedElement;
        // Check if the focused element is a descendant of the element controlled by the tab item.
        while (checkFocusElement) {
            if (tabPanel == checkFocusElement)
                return true;
            checkFocusElement = checkFocusElement->parentObject();
        }
    }

    return false;
}

AccessibilityObject* AccessibilityRenderObject::internalLinkElement() const
{
    Element* element = anchorElement();
    if (!element)
        return 0;

    // Right now, we do not support ARIA links as internal link elements
    if (!isHTMLAnchorElement(element))
        return 0;
    HTMLAnchorElement* anchor = toHTMLAnchorElement(element);

    KURL linkURL = anchor->href();
    String fragmentIdentifier = linkURL.fragmentIdentifier();
    if (fragmentIdentifier.isEmpty())
        return 0;

    // check if URL is the same as current URL
    KURL documentURL = m_renderer->document()->url();
    if (!equalIgnoringFragmentIdentifier(documentURL, linkURL))
        return 0;

    Node* linkedNode = m_renderer->document()->findAnchor(fragmentIdentifier);
    if (!linkedNode)
        return 0;

    // The element we find may not be accessible, so find the first accessible object.
    return firstAccessibleObjectFromNode(linkedNode);
}

AccessibilityObject* AccessibilityRenderObject::accessibilityImageMapHitTest(HTMLAreaElement* area, const IntPoint& point) const
{
    if (!area)
        return 0;

    AccessibilityObject* parent = axObjectCache()->getOrCreate(area->imageElement());
    if (!parent)
        return 0;

    AccessibilityObject::AccessibilityChildrenVector children = parent->children();
    unsigned count = children.size();
    for (unsigned k = 0; k < count; ++k) {
        if (children[k]->elementRect().contains(point))
            return children[k].get();
    }

    return 0;
}

bool AccessibilityRenderObject::renderObjectIsObservable(RenderObject* renderer) const
{
    // AX clients will listen for AXValueChange on a text control.
    if (renderer->isTextControl())
        return true;

    // AX clients will listen for AXSelectedChildrenChanged on listboxes.
    Node* node = renderer->node();
    if (nodeHasRole(node, "listbox") || (renderer->isBoxModelObject() && toRenderBoxModelObject(renderer)->isListBox()))
        return true;

    // Textboxes should send out notifications.
    if (nodeHasRole(node, "textbox"))
        return true;

    return false;
}

RenderObject* AccessibilityRenderObject::renderParentObject() const
{
    if (!m_renderer)
        return 0;

    RenderObject* parent = m_renderer->parent();

    // Case 1: node is a block and is an inline's continuation. Parent
    // is the start of the continuation chain.
    RenderObject* startOfConts = 0;
    RenderObject* firstChild = 0;
    if (m_renderer->isRenderBlock() && (startOfConts = startOfContinuations(m_renderer)))
        parent = startOfConts;

    // Case 2: node's parent is an inline which is some node's continuation; parent is
    // the earliest node in the continuation chain.
    else if (parent && parent->isRenderInline() && (startOfConts = startOfContinuations(parent)))
        parent = startOfConts;

    // Case 3: The first sibling is the beginning of a continuation chain. Find the origin of that continuation.
    else if (parent && (firstChild = parent->firstChild()) && firstChild->node()) {
        // Get the node's renderer and follow that continuation chain until the first child is found
        RenderObject* nodeRenderFirstChild = firstChild->node()->renderer();
        while (nodeRenderFirstChild != firstChild) {
            for (RenderObject* contsTest = nodeRenderFirstChild; contsTest; contsTest = nextContinuation(contsTest)) {
                if (contsTest == firstChild) {
                    parent = nodeRenderFirstChild->parent();
                    break;
                }
            }
            if (firstChild == parent->firstChild())
                break;
            firstChild = parent->firstChild();
            if (!firstChild->node())
                break;
            nodeRenderFirstChild = firstChild->node()->renderer();
        }
    }

    return parent;
}

bool AccessibilityRenderObject::isDescendantOfElementType(const QualifiedName& tagName) const
{
    for (RenderObject* parent = m_renderer->parent(); parent; parent = parent->parent()) {
        if (parent->node() && parent->node()->hasTagName(tagName))
            return true;
    }
    return false;
}

bool AccessibilityRenderObject::isSVGImage() const
{
    return remoteSVGRootElement();
}

void AccessibilityRenderObject::detachRemoteSVGRoot()
{
    if (AccessibilitySVGRoot* root = remoteSVGRootElement())
        root->setParent(0);
}

AccessibilitySVGRoot* AccessibilityRenderObject::remoteSVGRootElement() const
{
    if (!m_renderer || !m_renderer->isRenderImage())
        return 0;

    CachedImage* cachedImage = toRenderImage(m_renderer)->cachedImage();
    if (!cachedImage)
        return 0;

    Image* image = cachedImage->image();
    if (!image || !image->isSVGImage())
        return 0;

    SVGImage* svgImage = static_cast<SVGImage*>(image);
    FrameView* frameView = svgImage->frameView();
    if (!frameView)
        return 0;
    Frame* frame = frameView->frame();
    if (!frame)
        return 0;

    Document* doc = frame->document();
    if (!doc || !doc->isSVGDocument())
        return 0;

    SVGSVGElement* rootElement = toSVGDocument(doc)->rootElement();
    if (!rootElement)
        return 0;
    RenderObject* rendererRoot = rootElement->renderer();
    if (!rendererRoot)
        return 0;

    AccessibilityObject* rootSVGObject = frame->document()->axObjectCache()->getOrCreate(rendererRoot);

    // In order to connect the AX hierarchy from the SVG root element from the loaded resource
    // the parent must be set, because there's no other way to get back to who created the image.
    ASSERT(rootSVGObject && rootSVGObject->isAccessibilitySVGRoot());
    if (!rootSVGObject->isAccessibilitySVGRoot())
        return 0;

    return toAccessibilitySVGRoot(rootSVGObject);
}

AccessibilityObject* AccessibilityRenderObject::remoteSVGElementHitTest(const IntPoint& point) const
{
    AccessibilityObject* remote = remoteSVGRootElement();
    if (!remote)
        return 0;

    IntSize offset = point - roundedIntPoint(elementRect().location());
    return remote->accessibilityHitTest(IntPoint(offset));
}

// The boundingBox for elements within the remote SVG element needs to be offset by its position
// within the parent page, otherwise they are in relative coordinates only.
void AccessibilityRenderObject::offsetBoundingBoxForRemoteSVGElement(LayoutRect& rect) const
{
    for (AccessibilityObject* parent = parentObject(); parent; parent = parent->parentObject()) {
        if (parent->isAccessibilitySVGRoot()) {
            rect.moveBy(parent->parentObject()->elementRect().location());
            break;
        }
    }
}

// Hidden children are those that are not rendered or visible, but are specifically marked as aria-hidden=false,
// meaning that they should be exposed to the AX hierarchy.
void AccessibilityRenderObject::addHiddenChildren()
{
    Node* node = this->node();
    if (!node)
        return;

    // First do a quick run through to determine if we have any hidden nodes (most often we will not).
    // If we do have hidden nodes, we need to determine where to insert them so they match DOM order as close as possible.
    bool shouldInsertHiddenNodes = false;
    for (Node* child = node->firstChild(); child; child = child->nextSibling()) {
        if (!child->renderer() && isNodeAriaVisible(child)) {
            shouldInsertHiddenNodes = true;
            break;
        }
    }

    if (!shouldInsertHiddenNodes)
        return;

    // Iterate through all of the children, including those that may have already been added, and
    // try to insert hidden nodes in the correct place in the DOM order.
    unsigned insertionIndex = 0;
    for (Node* child = node->firstChild(); child; child = child->nextSibling()) {
        if (child->renderer()) {
            // Find out where the last render sibling is located within m_children.
            AccessibilityObject* childObject = axObjectCache()->get(child->renderer());
            if (childObject && childObject->accessibilityIsIgnored()) {
                AccessibilityChildrenVector children = childObject->children();
                if (children.size())
                    childObject = children.last().get();
                else
                    childObject = 0;
            }

            if (childObject)
                insertionIndex = m_children.find(childObject) + 1;
            continue;
        }

        if (!isNodeAriaVisible(child))
            continue;

        unsigned previousSize = m_children.size();
        if (insertionIndex > previousSize)
            insertionIndex = previousSize;

        insertChild(axObjectCache()->getOrCreate(child), insertionIndex);
        insertionIndex += (m_children.size() - previousSize);
    }
}

void AccessibilityRenderObject::addTextFieldChildren()
{
    Node* node = this->node();
    if (!node || !node->hasTagName(inputTag))
        return;

    HTMLInputElement* input = toHTMLInputElement(node);
    HTMLElement* spinButtonElement = input->innerSpinButtonElement();
    if (!spinButtonElement || !spinButtonElement->isSpinButtonElement())
        return;

    AccessibilitySpinButton* axSpinButton = static_cast<AccessibilitySpinButton*>(axObjectCache()->getOrCreate(SpinButtonRole));
    axSpinButton->setSpinButtonElement(static_cast<SpinButtonElement*>(spinButtonElement));
    axSpinButton->setParent(this);
    m_children.append(axSpinButton);
}

void AccessibilityRenderObject::addImageMapChildren()
{
    RenderBoxModelObject* cssBox = renderBoxModelObject();
    if (!cssBox || !cssBox->isRenderImage())
        return;

    HTMLMapElement* map = toRenderImage(cssBox)->imageMap();
    if (!map)
        return;

    for (Element* current = ElementTraversal::firstWithin(map); current; current = ElementTraversal::next(current, map)) {
        // add an <area> element for this child if it has a link
        if (isHTMLAreaElement(current) && current->isLink()) {
            AccessibilityImageMapLink* areaObject = static_cast<AccessibilityImageMapLink*>(axObjectCache()->getOrCreate(ImageMapLinkRole));
            areaObject->setHTMLAreaElement(toHTMLAreaElement(current));
            areaObject->setHTMLMapElement(map);
            areaObject->setParent(this);
            if (!areaObject->accessibilityIsIgnored())
                m_children.append(areaObject);
            else
                axObjectCache()->remove(areaObject->axObjectID());
        }
    }
}

void AccessibilityRenderObject::addCanvasChildren()
{
    if (!node() || !node()->hasTagName(canvasTag))
        return;

    // If it's a canvas, it won't have rendered children, but it might have accessible fallback content.
    // Clear m_haveChildren because AccessibilityNodeObject::addChildren will expect it to be false.
    ASSERT(!m_children.size());
    m_haveChildren = false;
    AccessibilityNodeObject::addChildren();
}

void AccessibilityRenderObject::addAttachmentChildren()
{
    if (!isAttachment())
        return;

    // FrameView's need to be inserted into the AX hierarchy when encountered.
    Widget* widget = widgetForAttachmentView();
    if (!widget || !widget->isFrameView())
        return;

    AccessibilityObject* axWidget = axObjectCache()->getOrCreate(widget);
    if (!axWidget->accessibilityIsIgnored())
        m_children.append(axWidget);
}

void AccessibilityRenderObject::addRemoteSVGChildren()
{
    AccessibilitySVGRoot* root = remoteSVGRootElement();
    if (!root)
        return;

    root->setParent(this);

    if (root->accessibilityIsIgnored()) {
        AccessibilityChildrenVector children = root->children();
        unsigned length = children.size();
        for (unsigned i = 0; i < length; ++i)
            m_children.append(children[i]);
    } else
        m_children.append(root);
}

void AccessibilityRenderObject::ariaSelectedRows(AccessibilityChildrenVector& result)
{
    // Get all the rows.
    AccessibilityChildrenVector allRows;
    if (isTree())
        ariaTreeRows(allRows);
    else if (isAccessibilityTable() && toAccessibilityTable(this)->supportsSelectedRows())
        allRows = toAccessibilityTable(this)->rows();

    // Determine which rows are selected.
    bool isMulti = isMultiSelectable();

    // Prefer active descendant over aria-selected.
    AccessibilityObject* activeDesc = activeDescendant();
    if (activeDesc && (activeDesc->isTreeItem() || activeDesc->isTableRow())) {
        result.append(activeDesc);
        if (!isMulti)
            return;
    }

    unsigned count = allRows.size();
    for (unsigned k = 0; k < count; ++k) {
        if (allRows[k]->isSelected()) {
            result.append(allRows[k]);
            if (!isMulti)
                break;
        }
    }
}

bool AccessibilityRenderObject::elementAttributeValue(const QualifiedName& attributeName) const
{
    if (!m_renderer)
        return false;

    return equalIgnoringCase(getAttribute(attributeName), "true");
}

bool AccessibilityRenderObject::inheritsPresentationalRole() const
{
    // ARIA states if an item can get focus, it should not be presentational.
    if (canSetFocusAttribute())
        return false;

    // ARIA spec says that when a parent object is presentational, and it has required child elements,
    // those child elements are also presentational. For example, <li> becomes presentational from <ul>.
    // http://www.w3.org/WAI/PF/aria/complete#presentation
    if (roleValue() != ListItemRole && roleValue() != ListMarkerRole)
        return false;

    AccessibilityObject* parent = parentObject();
    if (!parent->isAccessibilityRenderObject())
        return false;

    Node* elementNode = static_cast<AccessibilityRenderObject*>(parent)->node();
    if (!elementNode || !elementNode->isElementNode())
        return false;

    QualifiedName tagName = toElement(elementNode)->tagQName();
    if (tagName == ulTag || tagName == olTag || tagName == dlTag)
        return parent->roleValue() == PresentationalRole;

    return false;
}

LayoutRect AccessibilityRenderObject::computeElementRect() const
{
    RenderObject* obj = m_renderer;

    if (!obj)
        return LayoutRect();

    if (obj->node()) // If we are a continuation, we want to make sure to use the primary renderer.
        obj = obj->node()->renderer();

    // absoluteFocusRingQuads will query the hierarchy below this element, which for large webpages can be very slow.
    // For a web area, which will have the most elements of any element, absoluteQuads should be used.
    // We should also use absoluteQuads for SVG elements, otherwise transforms won't be applied.
    Vector<FloatQuad> quads;

    if (obj->isText())
        toRenderText(obj)->absoluteQuads(quads, 0, RenderText::ClipToEllipsis);
    else if (isWebArea() || isSeamlessWebArea() || obj->isSVGRoot())
        obj->absoluteQuads(quads);
    else
        obj->absoluteFocusRingQuads(quads);

    LayoutRect result = boundingBoxForQuads(obj, quads);

    Document* document = this->document();
    if (document && document->isSVGDocument())
        offsetBoundingBoxForRemoteSVGElement(result);

    // The size of the web area should be the content size, not the clipped size.
    if ((isWebArea() || isSeamlessWebArea()) && obj->frame()->view())
        result.setSize(obj->frame()->view()->contentsSize());

    // Checkboxes and radio buttons include their label as part of their rect.
    if (isCheckboxOrRadio()) {
        HTMLLabelElement* label = labelForElement(toElement(m_renderer->node()));
        if (label && label->renderer()) {
            LayoutRect labelRect = axObjectCache()->getOrCreate(label)->elementRect();
            result.unite(labelRect);
        }
    }

    return result;
}

} // namespace WebCore
