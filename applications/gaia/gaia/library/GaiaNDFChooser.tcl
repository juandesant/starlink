#+
#  Name:
#     GaiaNDFChooser

#  Type of Module:
#     [incr Tk] class

#  Purpose:
#     Provides a toolbox for selecting an NDF stored in a container
#     file.

#  Description:

#     This class provides a selection chooser for picking an NDF
#     stored in a container file. It is different from just a plain
#     file chooser in that is understands multiple NDFs stored in the
#     same container file, and also allows the selection of a
#     "displayable" component (these are the NDF data, variance and
#     quality arrays).
#
#     This class is the NDF equivalent of SkyCatHDUChooser.

#  Invocations:
#
#        GaiaNDFChooser object_name [configuration options]
#
#     This creates an instance of a GaiaNDFChooser object. The return is
#     the name of the object.
#
#        object_name configure -configuration_options value
#
#     Applies any of the configuration options (after the instance has
#     been created).
#
#        object_name method arguments
#
#     Performs the given method on this object.

#  Configuration options:
#     See itk_option definitions below.

#  Methods:
#     See method definitions below.

#  Inheritance:
#     util::TopLevelWidget

#  Authors:
#     PWD: Peter Draper (STARLINK - Durham University)
#     {enter_new_authors_here}

#  History:
#     17-JAN_2000 (PWD):
#        Original version.
#     {enter_further_changes_here}

#-

#.

itk::usual GaiaNDFChooser {}

itcl::class gaia::GaiaNDFChooser {

   #  Inheritances:
   #  -------------
   inherit util::TopLevelWidget

   #  Constructor:
   #  ------------
   constructor {args} {

      #  Evaluate any options.
      eval itk_initialize $args
   }

   #  Destructor:
   #  -----------
   destructor  {
   }

   #  Methods:
   #  --------

   # This method is called after the options have been evaluated.
   protected method init {} {
      wm title $w_ "NDFs ($itk_option(-number))"
      wm iconname $w_ "NDFs ($itk_option(-number))"

      #  Get internal image handle
      set image_ [$itk_option(-image) get_image]

      #  Create the interface.
      make_table_
      make_image_table_
      make_buttons_
      make_short_help_

      #  And start it up.
      show_hdu_list
   }

   # Make the table component for displaying the NDF info
   protected method make_table_ {} {
      set headings [$image_ hdu listheadings]

      # TableList(n) widget for displaying the list of NDFs
      itk_component add table {
         set table_ [util::TableList $w_.table \
                        -title "NDF list for file: [$image_ cget -file]" \
                        -headings $headings \
                        -width [expr [string length $headings]*2]]
      }
      pack $itk_component(table) -side top -fill both -expand 1

      bind $table_.listbox <ButtonRelease-1> [code $this select_ndf_]
      bind $table_.listbox <Double-ButtonPress-1> [code $this set_ndf_ "data"]
   }

   #  Make a subwindow for displaying miniature versions of image
   #  extensions (XXX inefficient?).
   protected method make_image_table_ {} {

      # Frame (BLT table) used to display images in NDF components
      itk_component add image_table {
         set imagetab_ [frame $w_.imagetab]
      }
      pack $itk_component(image_table) -side top -fill x
   }

   #  Buttons to control NDF display.
   protected method make_buttons_ {} {

      # Button frame at bottom of window
      itk_component add buttons {
         frame $w_.buttons \
            -borderwidth 2 \
            -relief raised
      }
      itk_component add data {
         button $w_.buttons.data \
            -text Data \
            -command [code $this set_ndf_ data]
      }
      itk_component add var {
         button $w_.buttons.var \
            -text Variance \
            -command [code $this set_ndf_ variance]
      }
      itk_component add qual {
         button $w_.buttons.qual \
            -text Quality \
            -command [code $this set_ndf_ quality]
      }
      itk_component add close {
         button $w_.buttons.close \
            -text Close \
            -command "destroy $w_"
      }

      pack $itk_component(data) -side left -expand 1
      pack $itk_component(var) -side left -expand 1
      pack $itk_component(qual) -side left -expand 1
      pack $itk_component(close) -side left -expand 1
      pack $itk_component(buttons) -side top -fill x -expand 1
   }

   #  Add a short help window
   protected method make_short_help_ {} {
      TopLevelWidget::make_short_help

      add_short_help $itk_component(table) \
         {Table: Click to select NDF, double-click to display image}
      add_short_help $itk_component(buttons).data \
         {Open and display the selected NDF data component}
      add_short_help $itk_component(buttons).var \
         {Open and display the selected NDF variance component}
      add_short_help $itk_component(buttons).qual \
         {Open and display the selected NDF quality component}
      add_short_help $itk_component(buttons).close {Close window}
   }

   #  Update the list of NDFs and the image displays, if needed
   public method show_hdu_list {} {
      set old_filename $filename_
      set filename_ [$image_ cget -file]
      set ndf_list [$image_ hdu list]
      set headings [$image_ hdu listheadings]

      #  Update the table listing
      $table_ clear
      $table_ config -height [llength $ndf_list] -info $ndf_list
      if {"$filename_" == "$old_filename"} {
         return
      }

      #  Delete old images
      set w $imagetab_.f
      catch {destroy $w}
      pack [frame $w] -fill x -expand 1
      if {"$filename_" == ""} {
         return
      }

      #  See if there is more than one image, otherwise skip it.
      set num_images_ 0
      catch {unset ext_}
      foreach ndf $ndf_list {
         eval lassign [list $ndf] $headings
         set ext_($num_images_,ndf) $number
         set ext_($num_images_,name) $name
         incr num_images_
      }
      if {$num_images_ <= 1} {
         return
      }

      #  Put the images in the table
      if { $itk_option(-display_images) } {
         blt::table $w
         for {set i 0} {$i < $num_images_} {incr i} {
            set f [frame $w.f$i -borderwidth 5 -relief raised]
            set im [RtdImage $f.im \
                       -graphics 0 \
                       -file $filename_ \
                       -canvaswidth 100 \
                       -canvasheight 100 \
                       -fitwidth 100 \
                       -fitheight 100]
            pack $im -fill x -expand 1

            #  Save widget names for later reference
            set ext_($i,frame) $f
            set ext_($i,RtdImage) $im
            set ext_($i,image) [$im get_image]
            set ext_($i,canvas) [$im get_canvas]

            set ndf $ext_($i,ndf)
            set name $ext_($i,name)

            #  Use after to override default bindings, etc.
            after 0 [code $this add_image_bindings_ $im $ndf $name]

            #  Position the image in the table.
            blt::table $w $f 0,$i -fill both
         }
      }

      # Select the NDF being displayed, if any
      select_image_ndf_ [$image_ hdu]
   }

   #  This method is called when the user clicks on an image NDF icon.
   #  Display the selected image.
   protected method select_image_ndf_ {ndf {component "data"}} {
      busy {
         $image_ hdu $ndf $component
         puts "fullname = [$image_ fullname]"
         for {set i 0} {$i < $num_images_} {incr i} {
            if {[info exists ext_($i,frame)]} {
               if {"$ext_($i,ndf)" == "$ndf"} {
                  $ext_($i,frame) configure -relief sunken
               } else {
                  $ext_($i,frame) configure -relief raised
               }
            }
         }
         update
         catch "$table_ select_row [expr $ndf-1]"
         select_ndf_
      }
   }

   #  Add bindings to the given RtdImage itcl class object and set it to
   #  display the given NDF when clicked on. Note this always displays
   #  the "data" component.
   protected method add_image_bindings_ {im ndf name} {
      set image [$im get_image]
      set canvas [$im get_canvas]

      #  Set the NDF for the image
      busy {
         $image hdu $ndf "data"
      }

      #  Need to add 2 bindings: one for the image, one for the background
      bind $canvas <1> [code $this select_image_ndf_ $ndf "data"]

      #  Set up a resize handler to change the image size
      bind $canvas <Configure> [code $this resize_ $im %w %h]

      #  Add a help message indicating which image it is
      set s $name
      if {"$s" != ""} {
         set s "($s)"
      }
      add_short_help $canvas "Click here to display NDF $ndf $s"
   }

   # This method is called when the image window is resized.
   # The rtdImage widget and the new width and height are given.
   protected method resize_ {im new_width new_height} {
      set image [$im get_image]
      $image config -fitwidth $new_width -fitheight $new_height
      $im center
   }

   #  Set the NDF to display. Makes the currently selected NDF the
   #  current NDF.
   protected method set_ndf_ {component} {
      set sel [$table_ get_selected]
      if {[llength $sel]} {
         lassign [lindex $sel 0] number
         select_image_ndf_ $number $component
      }
   }

   #  This method is called when a line in the NDF list is selected.
   protected method select_ndf_ {} {
      set sel [$table_ get_selected]
      if {[llength $sel]} {
         lassign [lindex $sel 0] number name naxis1 naxis2 havvar havqual
         if { "$havvar" == "true" } {
            $itk_component(var) configure -state normal
         } else {
            $itk_component(var) configure -state disabled
         }
         if { "$havqual" == "true" } {
            $itk_component(qual) configure -state normal
         } else {
            $itk_component(qual) configure -state disabled
         }
      }
   }

   #  Configuration options: (public variables)
   #  ----------------------

   #  Target GaiaCtrlImage.
   itk_option define -image image Image {}

   #  Whether to display small images.
   itk_option define -display_images display_images Display_Images 0 {
      if { $itk_option(-display_images) } {
         show_hdu_list
      }
   }

   #  Protected variables: (available to instance)
   #  --------------------

   #  Internal rtdimage object.
   protected variable image_

   #  Table displaying the NDFs.
   protected variable table_

   #  Table displaying image extensions.
   protected variable imagetab_

   #  Name of image file.
   protected variable filename_ {}

   #  Number of displayable NDFs in the current HDS container file.
   protected variable num_images_ 0

   #  Array(ndf,keyword) of image keyword and widget info
   protected variable ext_

   #  Common variables: (shared by all instances)
   #  -----------------


   protected variable component_ "data"

#  End of class definition.
}
