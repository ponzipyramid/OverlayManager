Scriptname OverlayManager Hidden

; High-Level Overlay API
int function AddOverlay(Actor akTarget, string asContext, string asSection, string asName, int aiColor = 0, float afAlpha = 1.0, int aiGlow = 0, int aiGloss = 0, string asBump = "") global native
bool function RemoveOverlay(Actor akTarget, string asContext, string asSection, string asName) global native
bool function SyncOverlays(Actor akTarget) global native

; Low-Level Overlay API
bool function ApplyOverlay(Actor akTarget, string asArea, int aiSlot, string asPath, int aiColor = 0, float afAlpha = 1.0, int aiGlow = 0, bool aiGloss = false, string asBump = "") global native
bool function ClearOverlay(Actor akTarget, string asArea, int aiSlot) global native
int[] function GetAvailableSlots(Actor akTarget, string asArea, int aiNeeded = 1) global native
int[] function GetExternalOverlaySlots(Actor akTarget, string asContext, string asArea) global native

function TrackActor(Actor akTarget) global native

; Magic Overlays
bool function ActivateOverlaySpell(Actor akTarget, string asContext, string asSection, string asName) global native
bool function DeactivateOverlaySpell(Actor akTarget, string asContext, string asSection, string asName) global native
bool function DeactivateAllOverlaySpells(Actor akTarget, string asContext) global native
bool function RefreshAllOverlaySpells(Actor akTarget, string asContext, int aiTemplate) global native

; JC API
bool function AreOverlaysSame(int aiTemplate, int aiTattoo, bool abConfigurable = false) global native
int function FindOverlayObject(int aiOverlayList, int aiOverlay) global native
int function GetOverlayObject(string asContext, string asSection, string asName) global native
int function IsValidOverlayObject(int aiOverlay) global native 
Form function GetOverlayForm(int aiOverlay, string asPlugin, int formId, Form akDefault) global native
int[] function GetAppliedOverlayObjects(Actor akTarget, string asArea) global native
int function GetAppliedOverlayInSlot(Actor akTarget, string asArea, int aiSlot) global native
int function AddOverlayObject(Actor akTarget, int aiOverlay) global native
int function FindOverlayConflict(int aiList, int aiTattoo) global native
int function FindOverlayRequirement(int aiList, int aiTattoo) global native
bool function RemoveMatchingTattoos(Actor akTarget, int aiTemplate, bool abIgnoreLock = false) global native
function QueryAvailableTattoos(int aiTemplate, int aiMatches, string asContext = "", int aiApplied = 0) global native
function QueryAppliedTattoos(Actor akTarget, int aiTemplate, int aiMatches, string asContext = "", string[] asExceptAreas, int[] asExceptSlots) global native