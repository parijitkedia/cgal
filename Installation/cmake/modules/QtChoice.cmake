macro(qt_automoc)
  if(QT5)
  	message(STATUS "Qt5 doesn't need automoc.")
  elseif(QT4)
   	qt4_automoc(${ARGN})
  else()
        message("No Qt selected for add_resources.")
  endif()
endmacro()

macro(qt_wrap_ui)
  if(QT5)
     	qt5_wrap_ui(${ARGN})
  elseif(QT4)
   	qt4_wrap_ui(${ARGN})
  else()
   	message("No Qt selected for wrap_ui.")
  endif()
endmacro()

macro(qt_wrap_cpp)
  if(QT5)
     qt5_wrap_cpp(${ARGN})
  elseif(QT4)
     qt4_wrap_cpp(${ARGN})
  else()
   message("No Qt selected for wrap_cpp.")
  endif()
endmacro()

macro(qt_generate_moc)
  if(QT5)
     qt5_generate_moc(${ARGN})
  elseif(QT4)
     qt4_generate_moc(${ARGN})
  else()
   message("No Qt selected for generate_moc.")
  endif()
endmacro()

macro(qt_add_dbus_adaptor)
  if(QT5)
     qt5_add_dbus_adaptor(${ARGN})
  elseif(QT4)
     qt4_add_dbus_adaptor(${ARGN})
  else()
   message("No Qt selected for add_dbus_adaptor.")
  endif()
endmacro()

macro(qt_add_dbus_interfaces)
  if(QT5)
      qt5_add_dbus_interfaces(${ARGN})
  elseif(QT4)
      qt4_add_dbus_interfaces(${ARGN})
  else()
   message("No Qt selected for add_dbus_interfaces.")
  endif()
endmacro()

macro(qt_add_dbus_interface)
  if(QT5)
  	qt5_add_dbus_interface(${ARGN})
  elseif(QT4)
  	qt4_add_dbus_interface(${ARGN})
  else()
   message("No Qt selected for add_dbus_interface.")
  endif()
endmacro()

macro(qt_generate_dbus_interface)
  if(QT5)
  	qt5_generate_dbus_interface(${ARGN})
  elseif(QT4)
  	qt4_generate_dbus_interface(${ARGN})
  else()
   message("No Qt selected for generate_dbus_interface.")
  endif()
endmacro()

macro(qt_add_resources)
  if(QT5)
  	qt5_add_resources(${ARGN})
  elseif(QT4)
   	qt4_add_resources(${ARGN})
  else()
   message("No Qt selected for add_resources.")
  endif()
endmacro()

