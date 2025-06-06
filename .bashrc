# PLEASE DO NOT MODIFY OR REMOVE THIS FILE!
# THIS IS IMPORTANT FOR THE BUILD STRUCTURE       

if [ -z "$PROJECT_ROOT" ]; then
  PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
fi

CAPSTONE_LIB="$PROJECT_ROOT/libs/capstone/build"
CAPSTONE_INCLUDE="$PROJECT_ROOT/libs/capstone/include"
CAPSTONE_PKGCONFIG="$CAPSTONE_LIB/pkgconfig"

LOCAL_LIB="$HOME/.local/lib"
LOCAL_INCLUDE="$HOME/.local/include"
LOCAL_PKGCONFIG="$LOCAL_LIB/pkgconfig"

case ":$PKG_CONFIG_PATH:" in
  *":$LOCAL_PKGCONFIG:"*) ;;
  *) export PKG_CONFIG_PATH="$LOCAL_PKGCONFIG${PKG_CONFIG_PATH:+:$PKG_CONFIG_PATH}" ;;
esac

case ":$PKG_CONFIG_PATH:" in
  *":$CAPSTONE_PKGCONFIG:"*) ;;
  *) export PKG_CONFIG_PATH="$CAPSTONE_PKGCONFIG${PKG_CONFIG_PATH:+:$PKG_CONFIG_PATH}" ;;
esac

case ":$LD_LIBRARY_PATH:" in
  *":$LOCAL_LIB:"*) ;;
  *) export LD_LIBRARY_PATH="$LOCAL_LIB${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" ;;
esac

case ":$LD_LIBRARY_PATH:" in
  *":$CAPSTONE_LIB:"*) ;;
  *) export LD_LIBRARY_PATH="$CAPSTONE_LIB${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" ;;
esac

case ":$CPATH:" in
  *":$LOCAL_INCLUDE:"*) ;;
  *) export CPATH="$LOCAL_INCLUDE${CPATH:+:$CPATH}" ;;
esac

case ":$CPATH:" in
  *":$CAPSTONE_INCLUDE:"*) ;;
  *) export CPATH="$CAPSTONE_INCLUDE${CPATH:+:$CPATH}" ;;
esac

case ":$LIBRARY_PATH:" in
  *":$LOCAL_LIB:"*) ;;
  *) export LIBRARY_PATH="$LOCAL_LIB${LIBRARY_PATH:+:$LIBRARY_PATH}" ;;
esac

case ":$LIBRARY_PATH:" in
  *":$CAPSTONE_LIB:"*) ;;
  *) export LIBRARY_PATH="$CAPSTONE_LIB${LIBRARY_PATH:+:$LIBRARY_PATH}" ;;
esac
