#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define M_SCI static const constexpr inline
#define MF_SCI static constexpr inline

namespace Constants
{
    struct Map
    {
        using XYPosition = ::std::pair<::std::int32_t, ::std::int32_t>;
        M_SCI::std::uint64_t sightRadius = 5000;									
        M_SCI::std::uint64_t sightRadiusSquared = sightRadius * sightRadius;		
        M_SCI::std::int32_t numOfGridPerCell = 1000;

		[[nodiscard]] MF_SCI auto CellToGrid(int x, int y) noexcept				// ��ȡָ���������ĵ�����
		{
			return ::std::make_pair<::std::int32_t, ::std::int32_t>(x * numOfGridPerCell + numOfGridPerCell / 2, y * numOfGridPerCell + numOfGridPerCell / 2);
		}

		[[nodiscard]] MF_SCI::std::int32_t GridToCellX(XYPosition pos) noexcept	// ��ȡָ���������λ�ڵĸ��ӵ� X ���
		{
			return pos.first / numOfGridPerCell;
		}

		[[nodiscard]] MF_SCI::std::int32_t GridToCellY(XYPosition pos) noexcept	// ��ȡָ���������λ�ڵĸ��ӵ� Y ���
		{
			return pos.second / numOfGridPerCell;
		}
    };
}
 
#endif // !CONSTANTS_H
