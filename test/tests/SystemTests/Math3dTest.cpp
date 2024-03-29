#include <math.h>

#include "test_config.h"
#include "Math3d.h"

TEST_CASE("Matrix multiply tests", "[System][Math]") {
    
    SECTION( "Identity matrix test") {
        Matrix3d matrix({{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}});
        Vec3 vec = Vec3 { 1, 2, 3 };

        Vec3 result = matrix.multiply(vec);

        REQUIRE( result.x == vec.x );
        REQUIRE( result.y == vec.y );
        REQUIRE( result.z == vec.z );
    }

    SECTION("simple test matrices") {
        Matrix3d matrix1({{{1, 0, 1}, {1, 1, 0}, {0, 1, 1}}});
        Matrix3d matrix2({{{0, 0, 0}, {1, 1, 1}, {1, 1, 1}}});
        Vec3 vec = Vec3 { 1, 2, 3 };

        Vec3 result1 = matrix1.multiply(vec);
        Vec3 result2 = matrix2.multiply(vec);

        REQUIRE( result1.x == vec.x + vec.y );
        REQUIRE( result1.y == vec.y + vec.z );
        REQUIRE( result1.z == vec.x + vec.z );

        REQUIRE( result2.x == vec.y + vec.z );
        REQUIRE( result2.y == vec.y + vec.z );
        REQUIRE( result2.z == vec.y + vec.z );
    }

    SECTION( "invert vector about origin") {
        Matrix3d matrix({{{-1, 0, 0}, {0, -1, 0}, {0, 0, -1}}});
        Vec3 vec = Vec3 { 1, 2, 3 };

        Vec3 result = matrix.multiply(vec);

        REQUIRE( result.x == -vec.x );
        REQUIRE( result.y == -vec.y );
        REQUIRE( result.z == -vec.z );
    }

    SECTION( "project vector onto orthgonal planes" ) {
        Matrix3d xyPlaneMatrix({{{1, 0, 0}, {0, 1, 0}, {0, 0, 0}}});
        Matrix3d yzPlaneMatrix({{{0, 0, 0}, {0, 1, 0}, {0, 0, 1}}});
        Matrix3d xzPlaneMatrix({{{1, 0, 0}, {0, 0, 0}, {0, 0, 1}}});
        Vec3 vec = Vec3 { 1, 2, 3 };

        Vec3 projectedXY = xyPlaneMatrix.multiply(vec);
        Vec3 projectedYZ = yzPlaneMatrix.multiply(vec);
        Vec3 projectedXZ = xzPlaneMatrix.multiply(vec);

        REQUIRE( projectedXY.x == vec.x );
        REQUIRE( projectedXY.y == vec.y );
        REQUIRE( projectedXY.z == 0 );

        REQUIRE( projectedYZ.x == 0 );
        REQUIRE( projectedYZ.y == vec.y );
        REQUIRE( projectedYZ.z == vec.z );

        REQUIRE( projectedXZ.x == vec.x );
        REQUIRE( projectedXZ.y == 0 );
        REQUIRE( projectedXZ.z == vec.z );
    }

    SECTION( "simple translation matric: x -> z, y -> x, z -> y" ) {
        Matrix3d matrix({{{0, 0, 1}, {1, 0, 0}, {0, 1, 0}}});
        Vec3 vec = Vec3 { 2, 5, 7 };
        
        Vec3 result = matrix.multiply(vec);

        REQUIRE( result.x == vec.y );
        REQUIRE( result.y == vec.z );
        REQUIRE( result.z == vec.x );
    }

    SECTION( "simple translation matrix: -x -> z, y -> x, -z -> y" ) {
        Matrix3d matrix({{{0, 0, -1}, {1, 0, 0}, {0, -1, 0}}});
        Vec3 vec = Vec3 { 2, 5, 7 };
        
        Vec3 result = matrix.multiply(vec);

        REQUIRE( result.x == vec.y );
        REQUIRE( result.y == -vec.z );
        REQUIRE( result.z == -vec.x );
    }
}