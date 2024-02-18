Scriptname OverlayManager Hidden

float function GetVersion()
    return 0.00010001
endFunction

; Unimplemented
; High-Level Overlay API
; string[] function QueryOverlays(string asDomain = "", string asArea = "", string asSet = "", string asName = "", string asTags = "", int aiFilterMode = 0, string[] asSlots = none) global native
; string[] function QueryAppliedOverlays(Actor akTarget, string asContext = "", string asArea = "", string asSet = "", string asName = "", string asTags = "", string[] asSlots = none) global native

; string function GetOverlayId(string asDomain, string asSet, string asName) global native
; string[] function GetOverlaySections(string asId) global native

; bool function AddOverlay(Actor akTarget, string asContext, string asId, int aiColor = 0, float afAlpha = 1.0, int aiGlow = 0, int aiGloss = 0, string asBump = "", string asReplaceId = "") global native
; bool function EditOverlay(Actor akTarget, string asId, int aiColor = 0, float afAlpha = 1.0, int aiGlow = 0, int aiGloss = 0, string asBump = "") global native
; bool function RemoveOverlay(Actor akTarget, string asContext, string asId) global native

; ; Overlay Effects API
; bool function AddFadeEffect(Actor akTarget, string asId, bool abFadeIn, float afPeriod, float afOpacity, int aiMode = 0, bool abRemoveOvl = false) global native
; bool function RemoveFadeEffect(Actor akTarget, string asId) global native

; bool function AddBreatheEffect(Actor akTarget, string asId, float afPeriod, float afMinOpacity, float afMaxOpacity, int aiReps = -1) global native
; bool function RemoveBreatheEffect(Actor akTarget, string asId) global native

; ; Low-Level Overlay API
; bool function ApplyOverlay(Actor akTarget, string asArea, int aiSlot, string asPath, int aiColor = 0, float afAlpha = 1.0, int aiGlow = 0, bool aiGloss = false, string asBump = "") global native
; bool function ClearOverlay(Actor akTarget, string asArea, int aiSlot) global native
; int[] function GetAvailableSlots(Actor akTarget, string asArea, int aiNeeded = 1) global native
; int[] function GetExternalOverlaySlots(Actor akTarget, string asContext, string asArea) global native

; Overlay Metadata
int function GetOverlayMetaInt(string asId, string asKey, int abDefault = 0) global native
string function GetOverlayMetaString(string asId, string asKey, string abDefault = "") global native
Form function GetOverlayMetaForm(string asId, string asKey, Form abDefault = none) global native

; Legacy ST Functions
bool function DoesOverlayMatch(int aiTemplate, int aiOverlay, bool abConsiderRuntime = false) global native
int[] function GetMatchingOverlays(int aiList, int aiTemplate, int aiMatches = 0) global native

int function GetOverlayObject(string asId) global native
function ConvertOverlayList(int aiList, string[] asIds) global native

function SyncContext(Actor akTarget, string asContext, int aiList, int aiAdded, int aiRemoved) global native