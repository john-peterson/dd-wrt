# HList.tcl --
#
#	This file defines the default bindings for Tix Hierarchical Listbox
#	widgets.
#
# Copyright (c) 1996, Expert Interface Technologies
#
# See the file "license.terms" for information on usage and redistribution
# of this file, and for a DISCLAIMER OF ALL WARRANTIES.
#



#--------------------------------------------------------------------------
# tkPriv elements used in this file:
#
# afterId -		Token returned by "after" for autoscanning.
# fakeRelease -		Cancel the ButtonRelease-1 after the user double click
#--------------------------------------------------------------------------
#
proc tixHListBind {} {
    tixBind TixHList <ButtonPress-1> {
	tixHList:Button-1 %W %x %y ""
    }
    tixBind TixHList <Shift-ButtonPress-1> {
	tixHList:Button-1 %W %x %y s
    }
    tixBind TixHList <Control-ButtonPress-1> {
	tixHList:Button-1 %W %x %y c
    }
    tixBind TixHList <ButtonRelease-1> {
	tixHList:ButtonRelease-1 %W %x %y
    }
    tixBind TixHList <Double-ButtonPress-1> {
	tixHList:Double-1 %W  %x %y
    }
    tixBind TixHList <B1-Motion> {
	set tkPriv(x) %x 
	set tkPriv(y) %y
	set tkPriv(X) %X
	set tkPriv(Y) %Y

	tixHList:B1-Motion %W %x %y
    }
    tixBind TixHList <B1-Leave> {
	set tkPriv(x) %x 
	set tkPriv(y) %y
	set tkPriv(X) %X
	set tkPriv(Y) %Y

	tixHList:B1-Leave %W
    }
    tixBind TixHList <B1-Enter> {
	tixHList:B1-Enter %W %x %y
    }

    # Keyboard bindings
    #
    tixBind TixHList <Up> {
	tixHList:UpDown %W prev ""
    }
    tixBind TixHList <Down> {
	tixHList:UpDown %W next ""
    }
    tixBind TixHList <Shift-Up> {
	tixHList:UpDown %W prev s
    }
    tixBind TixHList <Shift-Down> {
	tixHList:UpDown %W next s
    }
    tixBind TixHList <Left> {
	tixHList:LeftRight %W left
    }
    tixBind TixHList <Right> {
	tixHList:LeftRight %W right
    }
    tixBind TixHList <Prior> {
	%W yview scroll -1 pages
    }
    tixBind TixHList <Next> {
	%W yview scroll 1 pages
    }
    tixBind TixHList <Return> {
	tixHList:Keyboard-Activate %W 
    }
    tixBind TixHList <space> {
	tixHList:Keyboard-Browse %W 
    }
}

#----------------------------------------------------------------------
#
#
#			 Key bindings
#
#
#----------------------------------------------------------------------
proc tixHList:Keyboard-Activate {w} {
    if {[tixHList:GetState $w] != 0} {
	return
    }
    set ent [$w info anchor]

    if {$ent == ""} {
	return
    }

    if {[$w cget -selectmode] == "single"} {
	$w select clear
	$w select set $ent
    }

    set command [$w cget -command]
    if {$command != ""} {
	set bind(specs) {%V}
	set bind(%V)    $ent

	tixEvalCmdBinding $w $command bind $ent
    }
}

proc tixHList:Keyboard-Browse {w} {
    if {[tixHList:GetState $w] != 0} {
	return
    }
    set ent [$w info anchor]

    if {$ent == ""} {
	return
    }

    if {[$w cget -selectmode] == "single"} {
	$w select clear
	$w select set $ent
    }

    tixHList:Browse $w $ent
}

proc tixHList:LeftRight {w spec} {
    catch {
	uplevel #0 unset $w:priv:shiftanchor
    }
    if {[tixHList:GetState $w] != 0} {
	return
    }

    set anchor [$w info anchor]
    if {$anchor == ""} {
	set anchor [lindex [$w info children] 0]
    }
    if {$anchor == ""} {
	return
    }

    set ent $anchor
    while {1} {
	set e $ent
	if {$spec == "left"} {
	    set ent [$w info parent $e]

	    if {$ent == "" || [$w entrycget $ent -state] == "disabled"} {
		set ent [$w info prev $e]
	    }
	} else {
	    set ent [lindex [$w info children $e] 0]

	    if {$ent == "" || [$w entrycget $ent -state] == "disabled"} {
		set ent [$w info next $e]
	    }
	}

	if {$ent == ""} {
	    break
	}
 	if {[$w entrycget $ent -state] == "disabled"} {
	    continue
	}
 	if [$w info hidden $ent] {
	    continue
	}
	break
    }

    if {$ent == ""} {
       return
    }

    $w anchor set $ent
    $w see $ent

    if {[$w cget -selectmode] != "single"} {
	$w select clear
	$w selection set $ent

	tixHList:Browse $w $ent
    }
}

proc tixHList:UpDown {w spec mod} {
    if {[tixHList:GetState $w] != 0} {
	return
    }
    set anchor [$w info anchor]
    set done 0

    if {$anchor == ""} {
	set anchor [lindex [$w info children] 0]

	if {$anchor == ""} {
	    return
	}

	if {[$w entrycget $anchor -state] != "disabled"} {
	    # That's a good anchor
	    set done 1
	} else {
	    # We search for the first non-disabled entry (downward)
	    set spec next
	}
    }

    set ent $anchor
    # Find the prev/next non-disabled entry
    #
    while {!$done} {
	set ent [$w info $spec $ent]
	if {$ent == ""} {
	    break
	}
	if {[$w entrycget $ent -state] == "disabled"} {
	    continue
	}
	if [$w info hidden $ent] {
	    continue
	}
	break
    }

    if {$ent == ""} {
	return
    } else {
	$w see $ent
	$w anchor set $ent

	set selMode [$w cget -selectmode]
       if {$mod == "s" && ($selMode == "extended" || $selMode == "multiple")} {
	    global $w:priv:shiftanchor

	    if ![info exists $w:priv:shiftanchor] {
		set $w:priv:shiftanchor $anchor
	    }

	    $w selection clear
	    $w selection set $ent [set $w:priv:shiftanchor]
    
	    tixHList:Browse $w $ent
	} else {
	    catch {
		uplevel #0 unset $w:priv:shiftanchor
	    }

	    if {[$w cget -selectmode] != "single"} {
		$w select clear
		$w selection set $ent

		tixHList:Browse $w $ent
	    }
	}
    }
}

#----------------------------------------------------------------------
#
#
#			 Mouse bindings
#
#
#----------------------------------------------------------------------

proc tixHList:Button-1 {w x y mod} {
#    if {[$w cget -state] == "disabled"} {
#	return
#    }

    if [$w cget -takefocus] {
	focus $w
    }

    set selMode [$w cget -selectmode]

    case [tixHList:GetState $w] {
	{0} {
	    if {$mod == "s" && $selMode == "multiple"} {
		tixHList:GoState 28 $w $x $y
		return
	    }
	    if {$mod == "s" && $selMode == "extended"} {
		tixHList:GoState 28 $w $x $y
		return
	    }
	    if {$mod == "c" && $selMode == "extended"} {
		tixHList:GoState 33 $w $x $y
		return
	    }

	    tixHList:GoState 1 $w $x $y
	}
    }
}

proc tixHList:ButtonRelease-1 {w x y} {
    case [tixHList:GetState $w] {
	{5 16} {
	    tixHList:GoState 6 $w $x $y
	}
	{15} {
	    tixHList:GoState 17 $w $x $y
	}
	{10 11} {
	    tixHList:GoState 18 $w
	}
	{13 20} {
	    tixHList:GoState 14 $w $x $y
	}
	{21} {
	    tixHList:GoState 22 $w
	}
	{24} {
	    tixHList:GoState 25 $w
	}
	{26 28 33} {
	    tixHList:GoState 27 $w
	}
	{30} {
	    tixHList:GoState 32 $w
	}
    }
}

proc tixHList:Double-1 {w x y} {
    case [tixHList:GetState $w] {
	{0} {
	    tixHList:GoState 23 $w $x $y
	}
    }
}

proc tixHList:B1-Motion {w x y} {
    case [tixHList:GetState $w] {
	{1} {
	    tixHList:GoState 5 $w $x $y 
	}
	{5 16} {
	    tixHList:GoState 5 $w $x $y 
	}
	{13 20 21} {
	    tixHList:GoState 20 $w $x $y 
	}
	{24 26 28} {
	    tixHList:GoState 26 $w $x $y
	}
    }
}

proc tixHList:B1-Leave {w} {
    case [tixHList:GetState $w] {
	{5} {
	    tixHList:GoState 10 $w
	}
	{26} {
	    tixHList:GoState 29 $w
	}
    }
}

proc tixHList:B1-Enter {w x y} {
    case [tixHList:GetState $w] {
	{10 11} {
	    tixHList:GoState 12 $w $x $y
	}
	{29 30} {
	    tixHList:GoState 31 $w $x $y
	}
    }
}

proc tixHList:AutoScan {w} {
    case [tixHList:GetState $w] {
	{29 30} {
	    tixHList:GoState 30 $w
	}
    }
}

#----------------------------------------------------------------------
#
#			STATE MANIPULATION
#
#
#----------------------------------------------------------------------
proc tixHList:GetState {w} {
    global $w:priv:state

    if {![info exists $w:priv:state]} {
	set $w:priv:state 0
    }
    return [set $w:priv:state]
}

proc tixHList:SetState {w n} {
    global $w:priv:state

    set $w:priv:state $n
}

proc tixHList:GoState {n w args} {

#   puts "going from [tixHList:GetState $w] --> $n"

    tixHList:SetState $w $n
    eval tixHList:GoState-$n $w $args
}

#----------------------------------------------------------------------
#			States
#----------------------------------------------------------------------
proc tixHList:GoState-0 {w} {

}
proc tixHList:GoState-1 {w x y} {
    set oldEnt [$w info anchor]
    set ent [tixHList:SetAnchor $w $x $y 1]

    if {$ent == ""} {
	tixHList:GoState 0 $w
	return
    }

    set info [$w info item $x $y]
    if {[lindex $info 1] == "indicator"} {
	tixHList:GoState 13 $w $ent $oldEnt
    } else {
	if {[$w entrycget $ent -state] == "disabled"} {
	    tixHList:GoState 0 $w
	} else {
	    case [$w cget -selectmode] {
		{single browse} {
		    tixHList:GoState 16 $w $ent
		}
		default {
		    tixHList:GoState 24 $w $ent
		}
	    }
	}
    }
}

proc tixHList:GoState-5 {w x y} {
    set oldEnt [$w info anchor]

    set ent [tixHList:SetAnchor $w $x $y]

    if {$ent == "" || $oldEnt == $ent} {
	return
    }

    if {[$w cget -selectmode] != "single"} {
	tixHList:Select $w $ent
	tixHList:Browse $w $ent
    }
}

proc tixHList:GoState-6 {w x y} {
    set ent [tixHList:SetAnchor $w $x $y]

    if {$ent == ""} {
	tixHList:GoState 0 $w
	return
    }
    tixHList:Select $w $ent
    tixHList:Browse $w $ent

    tixHList:GoState 0 $w
}

proc tixHList:GoState-10 {w} {
    tixHList:StartScan $w
}

proc tixHList:GoState-11 {w} {
    global tkPriv

    tixHList:DoScan $w

    set oldEnt [$w info anchor]
    set ent [tixHList:SetAnchor $w $tkPriv(x) $tkPriv(y)]

    if {$ent == "" || $oldEnt == $ent} {
	return
    }

    if {[$w cget -selectmode] != "single"} {
	tixHList:Select $w $ent
	tixHList:Browse $w $ent
    }
}

proc tixHList:GoState-12 {w x y} {
    tkCancelRepeat
    tixHList:GoState 5 $w $x $y 
}

proc tixHList:GoState-13 {w ent oldEnt} {
    global tkPriv
    set tkPriv(tix,indicator) $ent
    set tkPriv(tix,oldEnt)    $oldEnt
    tixHList:CallIndicatorCmd $w <Arm> $ent
}

proc tixHList:GoState-14 {w x y} {
    global tkPriv

    if [tixHList:InsideArmedIndicator $w $x $y] {
	$w anchor set $tkPriv(tix,indicator)
	$w select clear
	$w select set $tkPriv(tix,indicator)
	tixHList:CallIndicatorCmd $w <Activate> $tkPriv(tix,indicator)
    } else {
	tixHList:CallIndicatorCmd $w <Disarm>   $tkPriv(tix,indicator)
    }

    unset tkPriv(tix,indicator)
    tixHList:GoState 0 $w
}

proc tixHList:GoState-16 {w ent} {
    if {$ent != "" && [$w cget -selectmode] != "single"} {
	tixHList:Select $w $ent
	tixHList:Browse $w $ent
    }
}

proc tixHList:GoState-18 {w} {
    global tkPriv
    tkCancelRepeat
    tixHList:GoState 6 $w $tkPriv(x) $tkPriv(y) 
}

proc tixHList:GoState-20 {w x y} {
    global tkPriv

    if {![tixHList:InsideArmedIndicator $w $x $y]} {
	tixHList:GoState 21 $w $x $y
    } else {
	tixHList:CallIndicatorCmd $w <Arm> $tkPriv(tix,indicator)
    }
}

proc tixHList:GoState-21 {w x y} {
    global tkPriv

    if {[tixHList:InsideArmedIndicator $w $x $y]} {
	tixHList:GoState 20 $w $x $y
    } else {
	tixHList:CallIndicatorCmd $w <Disarm> $tkPriv(tix,indicator)
    }
}

proc tixHList:GoState-22 {w} {
    global tkPriv

    if {$tkPriv(tix,oldEnt) != ""} {
	$w anchor set $tkPriv(tix,oldEnt)
    } else {
	$w anchor clear
    }
    tixHList:GoState 0 $w
}

proc tixHList:GoState-23 {w x y} {
    set ent [tixHList:GetNearest $w $y]

    if {$ent != ""} {
	set info [$w info item $x $y]

	if {[lindex $info 1] == "indicator"} {
	    tixHList:CallIndicatorCmd $w <Activate> $ent
	} else {
	    $w select set $ent
	    set command [$w cget -command]
	    if {$command != ""} {
		set bind(specs) {%V}
		set bind(%V)    $ent

		tixEvalCmdBinding $w $command bind $ent
	    }
	}
    }
    tixHList:GoState 0 $w
}

proc tixHList:GoState-24 {w ent} {
    if {$ent != ""} {
	tixHList:Select $w $ent
	tixHList:Browse $w $ent
    }
}

proc tixHList:GoState-25 {w} {
    set ent [$w info anchor]

    if {$ent != ""} {
	tixHList:Select $w $ent
	tixHList:Browse $w $ent
    }

    tixHList:GoState 0 $w
}


proc tixHList:GoState-26 {w x y} {
    set anchor [$w info anchor]
    if {$anchor == ""} {
	set first [lindex [$w info children ""] 0]
	if {$first != ""} {
	    $w anchor set $first
	    set anchor $first
	} else {
	    return
	}
    }

    set ent [tixHList:GetNearest $w $y 1]

    if {$ent != ""} {
	$w selection clear
	$w selection set $anchor $ent

	tixHList:Browse $w $ent
    }
}

proc tixHList:GoState-27 {w} {
    set ent [$w info anchor]

    if {$ent != ""} {
	tixHList:Browse $w $ent
    }

    tixHList:GoState 0 $w
}

proc tixHList:GoState-28 {w x y} {
    set anchor [$w info anchor]
    if {$anchor == ""} {
	set first [lindex [$w info children ""] 0]
	if {$first != ""} {
	    $w anchor set $first
	    set anchor $first
	} else {
	    return
	}
    }

    set ent [tixHList:GetNearest $w $y 1]
    if {$ent != ""} {
	$w selection clear
	$w selection set $anchor $ent

	tixHList:Browse $w $ent
    }
}

proc tixHList:GoState-29 {w} {
    tixHList:StartScan $w
}

proc tixHList:GoState-30 {w} {
    global tkPriv

    tixHList:DoScan $w

    set anchor [$w info anchor]
    if {$anchor == ""} {
	set first [lindex [$w info children ""] 0]
	if {$first != ""} {
	    $w anchor set $first
	    set anchor $first
	} else {
	    return
	}
    }

    set ent [tixHList:GetNearest $w $tkPriv(y) 1]
    if {$ent != ""} {
	$w selection clear
	$w selection set $anchor $ent

	tixHList:Browse $w $ent
    }
}

proc tixHList:GoState-31 {w x y} {
    tkCancelRepeat
    tixHList:GoState 26 $w $x $y 
}

proc tixHList:GoState-32 {w} {
    tkCancelRepeat
    tixHList:GoState 0 $w
}

proc tixHList:GoState-33 {w x y} {
    set ent [tixHList:GetNearest $w $y]
    if {$ent != ""} {
	$w anchor set $ent
	$w selection set $ent
	tixHList:Browse $w $ent
    }
}


#----------------------------------------------------------------------
#
#		Common actions
#
#----------------------------------------------------------------------
proc tixHList:GetNearest {w y {disableOK 0}} {
    set ent [$w nearest $y]

    if {$ent != ""} {
	if {!$disableOK && [$w entrycget $ent -state] == "disabled"} {
	    return ""
	}
    }

    return $ent
}

proc tixHList:SetAnchor {w x y {disableOK 0}} {
    set ent [tixHList:GetNearest $w $y $disableOK]

    if {$ent != ""} {
	if {[$w entrycget $ent -state] != "disabled"} {
	    $w anchor set $ent
	    $w see $ent
	    return $ent
	} elseif $disableOK {
	    return $ent
	}
    }

    return ""
}

proc tixHList:Select {w ent} {
    if {"x[$w info selection]" != "x$ent"} {
	$w selection clear
	$w select set $ent
    }
}

#----------------------------------------------------------------------
#
#		       Auto scan
#
#----------------------------------------------------------------------
proc tixHList:StartScan {w} {
    global tkPriv
    set tkPriv(afterId) [after 50 tixHList:AutoScan $w]
}

proc tixHList:DoScan {w} {
    global tkPriv
    set x $tkPriv(x)
    set y $tkPriv(y)
    set X $tkPriv(X)
    set Y $tkPriv(Y)

    if {$y >= [winfo height $w]} {
	$w yview scroll 1 units
    } elseif {$y < 0} {
	$w yview scroll -1 units
    } elseif {$x >= [winfo width $w]} {
	$w xview scroll 2 units
    } elseif {$x < 0} {
	$w xview scroll -2 units
    } else {
	return
    }

    set tkPriv(afterId) [after 50 tixHList:AutoScan $w]
}


#----------------------------------------------------------------------
#
#		Indicator handling
#
#----------------------------------------------------------------------

proc tixHList:CallIndicatorCmd {w event ent} {
    set cmd [$w cget -indicatorcmd]

    if {$cmd != ""} {
	set bind(type)  $event
	set bind(specs) {%V}
	set bind(%V)    $ent

	tixEvalCmdBinding $w $cmd bind $ent
    }
}

proc tixHList:InsideArmedIndicator {w x y} {
    global tkPriv

    set ent [tixHList:GetNearest $w $y 1]
    if {$ent == "" || $ent != $tkPriv(tix,indicator)} {
	return 0
    }

    set info [$w info item $x $y]
    if {[lindex $info 1] == "indicator"} {
	return 1
    } else {
	return 0
    }
}

proc tixHList:Browse {w ent} {
    set browsecmd [$w cget -browsecmd]
    if {$browsecmd != ""} {
	set bind(specs) {%V}
	set bind(%V)    $ent

	tixEvalCmdBinding $w $browsecmd bind $ent
    }
}


