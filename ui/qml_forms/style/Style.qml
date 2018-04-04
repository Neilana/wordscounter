pragma Singleton
import QtQuick 2.0

QtObject {

    // brown theme (terminal)
    //    property color highlightHoverMenuColor: '#98B579'
    //    property color highlightSelectedItemColor: '#E5C096'

    //    property color backgroundColor: '#E0DBD6' // white
    //    property color topBarBackgroundColor: '#CFCAC5' // white

    //    property color borderColor: '#88796B' // black
    //    property int borderWidth: 3

    //    property color buttonBorderColor: '#826D57' // black

    //    property color textColor: '#433B32'

    //    property color mainMenuSelectedColor: '#C1B8AF'

    // "Oceant sunset" (kuler)
    //property color highlightHoverMenuColor: '#E8B789'
    //property color highlightHoverMenuColor: '#9DBF93'
    //property color highlightHoverMenuColor: '#A3B97C'

    //property color highlightHoverMenuColor: '#E7B192'
    //property color highlightHoverMenuColor: '#E7BC92'
    //property color highlightHoverMenuColor: '#a67d81'
    //property color highlightHoverMenuColor: '#caaea9'
    //property color highlightHoverMenuColor: '#a08f91'

    //property color highlightHoverMenuColor: '#cbc8af'
    property color highlightHoverMenuColor: '#c4dd99'

    property color longButtonColor: '#CFCAC5'

    //property color highlightSelectedItemColor: '#FFD3B1'
    property color highlightSelectedItemColor: '#f1c492'

    property color backgroundColor: '#E0DBD6'

    property color topBarHighlightHoverColor: '#c4dd98' // green

    //property color topBarHighlightHoverColor: '#8EBF9B'
    property color topBarBackgroundColor: '#CFCAC5'

    property color borderColor: '#88796B'
    property int borderWidth: 3

    property color innerBorderColor: '#ABA096'
    property color buttonBorderColor: '#826D57'

    property color textColor: '#260707'

    //property color topSelectedColor: '#C3B6AE'
    property color topSelectedColor: '#bbb8ae'

    //property color mainMenuSelectedColor: '#A7A098'
    //property color mainMenuSelectedColor: '#d0bcb7'
    //property color mainMenuSelectedColor: '#b3a69d'
    //property color mainMenuSelectedColor: '#b4a29e'
    //property color mainMenuSelectedColor: '#BDB1B5'
    property color mainMenuSelectedColor: '#a1a096'

    //property color subMenuSelectedColor: '#C3B6AE'
    property color subMenuSelectedColor: '#c3c0b7'

    property int defaultFontPixelSize: 18

    // property font fontName: "/fonts/Cantarell-Regular.ttf"
    property FontLoader mainMenuFont: FontLoader {
        //name: "spartanFont"
        //        source: "/fonts/LeagueSpartan-Bold.otf"
        source: "/fonts/Geometric.otf"

        // source: "/fonts/Gilroy-ExtraBold.otf"
        // source: "/fonts/LibreBaskerville-Regular.ttf"
        // source: "/fonts/Montserrat-Black.otf"
    }

    property FontLoader subMenuFont: FontLoader {
        //name: "spartanFont"
        //source: "/fonts/LeagueSpartan-Bold.otf"
        //source: "/fonts/LibreBaskerville-Regular.ttf"

        //source: "/fonts/Montserrat-Regular.otf"
        //source: "/fonts/MuseoSansCyrl.otf"
        source: "/fonts/Geometric.otf"
    }
    property FontLoader tableFont: FontLoader {
        //name: "spartanFont"
        //source: "/fonts/LeagueSpartan-Bold.otf"
        //source: "/fonts/LibreBaskerville-Regular.ttf"

        //source: "/fonts/Montserrat-Regular.otf"
        source: "/fonts/MuseoSansCyrl.otf"
        //source: "/fonts/Geometric.otf"
    }

    property FontLoader readingFont: FontLoader {
        //source: "/fonts/LibreBaskerville-Regular.ttf"
        source: "/fonts/Heuristica-Regular.otf"
    }

    property FontLoader contentsFont: FontLoader {
        //source: "/fonts/LibreBaskerville-Regular.ttf"
        //source: "/fonts/Heuristica-Regular.otf"
        //source: "/fonts/Cantarell-Regular.ttf"
        source: "/fonts/MuseoSansCyrl.otf"
    }
    property int contentsFontPixelSize: 15
}
