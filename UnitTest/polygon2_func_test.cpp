#include "pch.h"

#include "../DesignLab/designlab_polygon.h"
#include "../DesignLab/designlab_polygon.cpp"


namespace dl_vec_test
{
	//addVertex�֐��̃e�X�g
	TEST(SPolygon2, AddVertex)
	{
		//�l�p�`���쐬���C���̒��_�̍��W���m�F����
		dl_vec::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		EXPECT_EQ(polygon.getVertexNum(), 4);
		EXPECT_EQ(polygon.getVertex(0), dl_vec::SVector2(0, 0));
		EXPECT_EQ(polygon.getVertex(1), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon.getVertex(2), dl_vec::SVector2(1, 1));
		EXPECT_EQ(polygon.getVertex(3), dl_vec::SVector2(0, 1));

		//6�p�`���쐬���C���̒��_�̍��W���m�F����
		dl_vec::SPolygon2 polygon2;
		polygon2.addVertex({ 0, 0 });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1 });
		polygon2.addVertex({ -1, 1 });
		polygon2.addVertex({ -1, 0 });

		EXPECT_EQ(polygon2.getVertexNum(), 6);
		EXPECT_EQ(polygon2.getVertex(0), dl_vec::SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), dl_vec::SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), dl_vec::SVector2(0, 1));
		EXPECT_EQ(polygon2.getVertex(4), dl_vec::SVector2(-1, 1));
		EXPECT_EQ(polygon2.getVertex(5), dl_vec::SVector2(-1, 0));
	}

	//addVertexCheckForDuplicates�֐��̃e�X�g
	TEST(SPolygon2, AddVertexCheckForDuplicates)
	{
		//�d�����钸�_��ǉ����C���̒��_�̍��W���m�F����
		dl_vec::SPolygon2 polygon;
		polygon.addVertexCheckForDuplicates({ 0, 0 });
		polygon.addVertexCheckForDuplicates({ 1, 0 });
		polygon.addVertexCheckForDuplicates({ 1, 1 });
		polygon.addVertexCheckForDuplicates({ 0, 1 });
		polygon.addVertexCheckForDuplicates({ 0, 0 });	//�d�����钸�_

		EXPECT_EQ(polygon.getVertexNum(), 4);
		EXPECT_EQ(polygon.getVertex(0), dl_vec::SVector2(0, 0));
		EXPECT_EQ(polygon.getVertex(1), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon.getVertex(2), dl_vec::SVector2(1, 1));
		EXPECT_EQ(polygon.getVertex(3), dl_vec::SVector2(0, 1));

		//�d�����Ȃ����_��ǉ����C���̒��_�̍��W���m�F����
		dl_vec::SPolygon2 polygon2;
		polygon2.addVertexCheckForDuplicates({ 0, 0 });
		polygon2.addVertexCheckForDuplicates({ 1, 0 });
		polygon2.addVertexCheckForDuplicates({ 1, 1 });
		polygon2.addVertexCheckForDuplicates({ 0, 1 });
		polygon2.addVertexCheckForDuplicates({ -1, 1 });

		EXPECT_EQ(polygon2.getVertexNum(), 5);
		EXPECT_EQ(polygon2.getVertex(0), dl_vec::SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), dl_vec::SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), dl_vec::SVector2(0, 1));
		EXPECT_EQ(polygon2.getVertex(4), dl_vec::SVector2(-1, 1));
	}

	//removeVertex�֐��̃e�X�g
	TEST(SPolygon2, RemoveVertex)
	{
		//�l�p�`���쐬���C���_���폜����
		dl_vec::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		polygon.removeVertex(0);
		EXPECT_EQ(polygon.getVertexNum(), 3);
		EXPECT_EQ(polygon.getVertex(0), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon.getVertex(1), dl_vec::SVector2(1, 1));
		EXPECT_EQ(polygon.getVertex(2), dl_vec::SVector2(0, 1));

		polygon.removeVertex(1);
		EXPECT_EQ(polygon.getVertexNum(), 2);
		EXPECT_EQ(polygon.getVertex(0), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon.getVertex(1), dl_vec::SVector2(0, 1));

		polygon.removeVertex(1);
		EXPECT_EQ(polygon.getVertexNum(), 1);
		EXPECT_EQ(polygon.getVertex(0), dl_vec::SVector2(1, 0));

		polygon.removeVertex(0);
		EXPECT_EQ(polygon.getVertexNum(), 0);

		//���݂��Ȃ����_���w�肵���ꍇ�������Ȃ��D�l�p�`���쐬���C���݂��Ȃ����_���w�肷��
		dl_vec::SPolygon2 polygon2;
		polygon2.addVertex({ 0, 0 });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1 });

		polygon2.removeVertex(4);
		EXPECT_EQ(polygon2.getVertexNum(), 4);
		EXPECT_EQ(polygon2.getVertex(0), dl_vec::SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), dl_vec::SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), dl_vec::SVector2(0, 1));

		polygon2.removeVertex(5);
		EXPECT_EQ(polygon2.getVertexNum(), 4);
		EXPECT_EQ(polygon2.getVertex(0), dl_vec::SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), dl_vec::SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), dl_vec::SVector2(0, 1));

		//���̒l���w�肵���ꍇ
		polygon2.removeVertex(-1);
		EXPECT_EQ(polygon2.getVertexNum(), 4);
		EXPECT_EQ(polygon2.getVertex(0), dl_vec::SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), dl_vec::SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), dl_vec::SVector2(0, 1));
	}

	//removeLastVertex�֐��̃e�X�g
	TEST(SPolygon2, RemoveLastVertex)
	{
		//�l�p�`���쐬���C���_���폜����
		dl_vec::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		polygon.removeLastVertex();
		EXPECT_EQ(polygon.getVertexNum(), 3);
		EXPECT_EQ(polygon.getVertex(0), dl_vec::SVector2(0, 0));
		EXPECT_EQ(polygon.getVertex(1), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon.getVertex(2), dl_vec::SVector2(1, 1));

		polygon.removeLastVertex();
		EXPECT_EQ(polygon.getVertexNum(), 2);
		EXPECT_EQ(polygon.getVertex(0), dl_vec::SVector2(0, 0));
		EXPECT_EQ(polygon.getVertex(1), dl_vec::SVector2(1, 0));

		polygon.removeLastVertex();
		EXPECT_EQ(polygon.getVertexNum(), 1);
		EXPECT_EQ(polygon.getVertex(0), dl_vec::SVector2(0, 0));

		polygon.removeLastVertex();
		EXPECT_EQ(polygon.getVertexNum(), 0);

		//6�p�`���쐬���C���_���폜����
		dl_vec::SPolygon2 polygon2;
		polygon2.addVertex({ 0, 0 });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1 });
		polygon2.addVertex({ -1, 1 });
		polygon2.addVertex({ -1, 0 });

		polygon2.removeLastVertex();
		EXPECT_EQ(polygon2.getVertexNum(), 5);
		EXPECT_EQ(polygon2.getVertex(0), dl_vec::SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), dl_vec::SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), dl_vec::SVector2(0, 1));
		EXPECT_EQ(polygon2.getVertex(4), dl_vec::SVector2(-1, 1));
	}

	//getVertexNum�֐��̃e�X�g
	TEST(SPolygon2, GetVertexNum)
	{
		//���_���Ȃ����p�`���쐬���C���_�����m�F����
		dl_vec::SPolygon2 polygon2;
		EXPECT_EQ(polygon2.getVertexNum(), 0);

		//4�p�`���쐬���C���_�����m�F����
		dl_vec::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		EXPECT_EQ(polygon.getVertexNum(), 4);

		//���_��S�č폜���C���_�����m�F����
		polygon.reset();

		EXPECT_EQ(polygon.getVertexNum(), 0);

		//6�p�`���쐬���C���_�����m�F����
		dl_vec::SPolygon2 polygon3;
		polygon3.addVertex({ 0, 0 });
		polygon3.addVertex({ 1, 0 });
		polygon3.addVertex({ 1, 1 });
		polygon3.addVertex({ 0, 1 });
		polygon3.addVertex({ -1, 1 });
		polygon3.addVertex({ -1, 0 });

		EXPECT_EQ(polygon3.getVertexNum(), 6);

		//���_���폜���C���_�����m�F����
		polygon3.removeVertex(0);
		EXPECT_EQ(polygon3.getVertexNum(), 5);
		polygon3.removeVertex(0);
		EXPECT_EQ(polygon3.getVertexNum(), 4);
		polygon3.removeVertex(0);
		EXPECT_EQ(polygon3.getVertexNum(), 3);
		polygon3.removeVertex(0);
		EXPECT_EQ(polygon3.getVertexNum(), 2);
		polygon3.removeVertex(0);
		EXPECT_EQ(polygon3.getVertexNum(), 1);
		polygon3.removeVertex(0);
		EXPECT_EQ(polygon3.getVertexNum(), 0);
	}

	//getVertex�֐��̃e�X�g
	TEST(SPolygon2, GetVertex)
	{
		//4�p�`���쐬���C���̒��_�̍��W���m�F����
		dl_vec::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		EXPECT_EQ(polygon.getVertex(0), dl_vec::SVector2(0, 0));
		EXPECT_EQ(polygon.getVertex(1), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon.getVertex(2), dl_vec::SVector2(1, 1));
		EXPECT_EQ(polygon.getVertex(3), dl_vec::SVector2(0, 1));

		//6�p�`���쐬���C���̒��_�̍��W���m�F����
		dl_vec::SPolygon2 polygon2;
		polygon2.addVertex({ 0, 0 });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1 });
		polygon2.addVertex({ -1, 1 });
		polygon2.addVertex({ -1, 0 });

		EXPECT_EQ(polygon2.getVertex(0), dl_vec::SVector2(0, 0));
		EXPECT_EQ(polygon2.getVertex(1), dl_vec::SVector2(1, 0));
		EXPECT_EQ(polygon2.getVertex(2), dl_vec::SVector2(1, 1));
		EXPECT_EQ(polygon2.getVertex(3), dl_vec::SVector2(0, 1));
		EXPECT_EQ(polygon2.getVertex(4), dl_vec::SVector2(-1, 1));
		EXPECT_EQ(polygon2.getVertex(5), dl_vec::SVector2(-1, 0));
	}

	//isConvex�֐��̃e�X�g
	TEST(SPolygon2, IsConvexRightTurn)
	{
		//4�p�`���쐬���C�ʑ��p�`���ǂ������m�F����
		dl_vec::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 1, 0 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 0, 1 });

		EXPECT_TRUE(polygon.isConvex());

		//6�p�`���쐬���C�ʑ��p�`���ǂ������m�F����
		dl_vec::SPolygon2 polygon2;
		polygon2.addVertex({ 0, -0.5f });
		polygon2.addVertex({ 1, 0 });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 0, 1.5f });
		polygon2.addVertex({ -1, 1 });
		polygon2.addVertex({ -1, 0 });

		EXPECT_TRUE(polygon2.isConvex());

		//�ʑ��p�`�łȂ����p�`���쐬���C�ʑ��p�`���ǂ������m�F����
		dl_vec::SPolygon2 polygon3;
		polygon3.addVertex({ 0, 0 });
		polygon3.addVertex({ 1, 0 });
		polygon3.addVertex({ 1, 1 });
		polygon3.addVertex({ 0, 1 });
		polygon3.addVertex({ 0.5f, 0.5f });

		EXPECT_FALSE(polygon3.isConvex());

		//���_����3�����̑��p�`���쐬���C�ʑ��p�`���ǂ������m�F����
		dl_vec::SPolygon2 polygon4;
		polygon4.addVertex({ 0, 0 });
		polygon4.addVertex({ 1, 0 });

		EXPECT_FALSE(polygon4.isConvex());
	}

	//isConvex�֐��̃e�X�g(�����)
	TEST(SPolygon2, IsConvexLeftTurn)
	{
		//4�p�`���쐬���C�ʑ��p�`���ǂ������m�F����
		dl_vec::SPolygon2 polygon;
		polygon.addVertex({ 0, 0 });
		polygon.addVertex({ 0, 1 });
		polygon.addVertex({ 1, 1 });
		polygon.addVertex({ 1, 0 });

		EXPECT_TRUE(polygon.isConvex());

		//6�p�`���쐬���C�ʑ��p�`���ǂ������m�F����
		dl_vec::SPolygon2 polygon2;
		polygon2.addVertex({ 0, -0.5f });
		polygon2.addVertex({ -1, 0 });
		polygon2.addVertex({ -1, 1 });
		polygon2.addVertex({ 0, 1.5f });
		polygon2.addVertex({ 1, 1 });
		polygon2.addVertex({ 1, 0 });

		EXPECT_TRUE(polygon2.isConvex());

		//�ʑ��p�`�łȂ����p�`���쐬���C�ʑ��p�`���ǂ������m�F����
		dl_vec::SPolygon2 polygon3;
		polygon3.addVertex({ 0, 0 });
		polygon3.addVertex({ 0, 1 });
		polygon3.addVertex({ 1, 1 });
		polygon3.addVertex({ 1, 0 });
		polygon3.addVertex({ 0.5f, 0.5f });


		EXPECT_FALSE(polygon3.isConvex());

		//���_����3�����̑��p�`���쐬���C�ʑ��p�`���ǂ������m�F����
		dl_vec::SPolygon2 polygon4;
		polygon4.addVertex({ 0, 0 });
		polygon4.addVertex({ 1, 0 });

		EXPECT_FALSE(polygon4.isConvex());
	}

}	//namespace dl_vec_test